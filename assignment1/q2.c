// prints a list of all international flights do not have a reciprocal return

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) 
{
	sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement

	int rc;

  	if( argc!=2 ) {
    	fprintf(stderr, "Usage: %s database\n", argv[0]);
    	return(1);
  	}

  	rc = sqlite3_open(argv[1], &db); //open a connnection to an SQLite database file
  	if( rc ) {
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}

    /******************************************************************
  	* select flights that are international by active and commerical **
    *******************************************************************/
  	char *sql_stmt = "create table international as select \
                      distinct r.* from routes r, airports p1, airports p2 \
                      where r.Source_airport_ID = p1.Airport_ID and \
                      r.Destination_airport_ID = p2.Airport_ID and \
                      p1.Country != p2.Country";
    rc = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

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

    /*****************************************************************************
  	* select flights that "do" have a reciprocal return and named as reciprocal **
    ******************************************************************************/
  	char *sql_stmt1 = "create table reciprocal as select \
                      distinct r1.* from routes r1, routes r2 \
                      where r1.Source_airport = r2.Destination_airport \
                      and r2.Source_airport = r1.Destination_airport \
                      and r1.Airline = r2.Airline";
    rc = sqlite3_prepare_v2(db, sql_stmt1, -1, &stmt, 0);
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

    /********************************************************
  	* select from international flights without reciprocal **
    *********************************************************/
  	char *sql_stmt2 = "create table notreciprocal as select \
                      distinct i.* from international i \
                      except select reci.* from reciprocal reci";
    rc = sqlite3_prepare_v2(db, sql_stmt2, -1, &stmt, 0);
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

    /**************************************************
    * connect notreciprocal table to airlines table  **
    ***************************************************/
    char *sql_stmt3 = "select nc.* from airlines l, notreciprocal nc \
                      where nc.Airline_ID = l.Airline_ID and \
                      l.Callsign != '' AND l.Active = 'Y'";
    rc = sqlite3_prepare_v2(db, sql_stmt3, -1, &stmt, 0);

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
    char *sql1 = "drop table international";
    rc = sqlite3_exec(db, sql1, 0, 0, &zErrMsg);  
    char *sql2 = "drop table reciprocal";
    rc = sqlite3_exec(db, sql2, 0, 0, &zErrMsg);  
    char *sql3 = "drop table notreciprocal";
    rc = sqlite3_exec(db, sql3, 0, 0, &zErrMsg);  

    sqlite3_close(db);

    return 0;
}