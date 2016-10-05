// prints the list of airports that can be reached from YEG with at most 3 connections

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) 
{
	sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement


	int rc;

  	if( argc!=2 ) {
    	fprintf(stderr, "Usage: %s <database> \n", argv[0]);
    	return(1);
  	}

  	rc = sqlite3_open(argv[1], &db); //open a connnection to an SQLite database file
  	if( rc ) {
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}

    /*********************************************************
    * select routes that can be reached from "YEG" directly **
    **********************************************************/
  	char *sql_stmt_1 = "create table A1 as select distinct r.* \
                        from airports p, routes r where Source_airport = 'YEG' \
                        and Source_airport_ID = Airport_ID and \
                        r.Stops <= 3 group by r.Destination_airport";
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

    /**************************************************************************
    * select routes that can be reached from "YEG" through 1 or 2 connection **
    ***************************************************************************/
    char *sql_stmt_2 = "create table A2 as select distinct r.* from routes r, A1 \
                        where A1.Destination_airport = r.Source_airport \
                        and A1.Stops+r.Stops+1<=3 \
                        group by r.Destination_airport";
   rc = sqlite3_prepare_v2(db, sql_stmt_2, -1, &stmt, 0);
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

    /*********************************************************************
    * select routes that can be reached from "YEG" at 2 or 3 connection **
    **********************************************************************/   
    char *sql_stmt_3 = "create table A3 as select distinct r.* from routes r, A2, A1 \
                        where A2.Destination_airport = r.Source_airport \
                        and A1.Stops+A2.Stops+r.Stops<=3 \
                        group by r.Destination_airport";
    rc = sqlite3_prepare_v2(db, sql_stmt_3, -1, &stmt, 0);
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

    /****************************************************************
    * select routes that can be reached from "YEG" at 3 connection **
    *****************************************************************/   
    char *sql_stmt_4 = "create table A4 as select distinct r.* from routes r, A3 \
                        where A3.Destination_airport = r.Source_airport \
                        and r.Stops = 0 group by r.Destination_airport";
    rc = sqlite3_prepare_v2(db, sql_stmt_4, -1, &stmt, 0);
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

    /*******************************************************
    * union the table before to get rid of duplicate data **
    ********************************************************/
    char *sql_stmt_5 = "create table result as select * from A4 \
                        union select * from A3 union select * from A2 \
                        union select * from A1";
    rc = sqlite3_prepare_v2(db, sql_stmt_5, -1, &stmt, 0);
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

    char *print = "select distinct p.Name from airports p, result r \
                  where r.Destination_airport_ID = p.Airport_ID;";
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
    char *sql1 = "drop table A1";
    rc = sqlite3_exec(db, sql1, 0, 0, &zErrMsg);  
    char *sql2 = "drop table A2";
    rc = sqlite3_exec(db, sql2, 0, 0, &zErrMsg);  
    char *sql3 = "drop table A3";
    rc = sqlite3_exec(db, sql3, 0, 0, &zErrMsg);  
    char *sql4 = "drop table A4";
    rc = sqlite3_exec(db, sql4, 0, 0, &zErrMsg);  
    char *sql5 = "drop table result";
    rc = sqlite3_exec(db, sql5, 0, 0, &zErrMsg);  
    
    sqlite3_close(db);

    return 0;
}

