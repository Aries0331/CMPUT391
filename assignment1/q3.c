// prints the list of top-10 countries with the most airlines flying to/from Canada 

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) 
{
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the select statement

	int rc;

  	if( argc!=2 ) {
    	fprintf(stderr, "Usage: %s <database file> \n", argv[0]);
    	return(1);
  	}

  	rc = sqlite3_open(argv[1], &db); //open a connnection to an SQLite database file
  	if( rc ) {
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}

  	/*******************************************************
    *  select flights whose destination or source airport **
    *  is Canada but not belong to Candan                 **
    ********************************************************/
    char *sql_stmt_1 = "create table s1 as select distinct Airline_ID \
                        from (select r.* from routes r, airports p where \
                        r.Source_airport_ID = p.Airport_ID and \
                        p.Country = 'Canada' \
                        union \
                        select r.* from routes r, airports p where \
                        r.Destination_airport_ID = p.Airport_ID and \
                        p.Country = 'Canada')";
    rc = sqlite3_prepare_v2(db, sql_stmt_1, -1, &stmt, 0);  
    if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col;
        for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
          printf("%s|", sqlite3_column_text(stmt, col));
        }
        printf("%s", sqlite3_column_text(stmt, col));
        printf("\n");
    }  
    sqlite3_finalize(stmt); //always finalize a statement

    /***************************
    * select top-10 airlines  **
    ****************************/
    char *print = "select Country, count(*) from s1 r, airlines l \
                    where r.Airline_ID = l.Airline_ID group by \
                    Country order by count(*) desc limit 10";
    rc = sqlite3_prepare_v2(db, print, -1, &stmt, 0);  
    if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col;
        for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
          printf("%s|", sqlite3_column_text(stmt, col));
        }
        printf("%s", sqlite3_column_text(stmt, col));
        printf("\n");
    }  
    sqlite3_finalize(stmt); //always finalize a statement

    char *zErrMsg = 0;
    char *sql = "drop table s1";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);  

    sqlite3_close(db);

    return 0;
}

