// prints the list of airports with commercial flights 
//into (or out of) them, but yet cannot be reached flying 
//from from YEG regardless of the number of connections

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

    /*************************************************************
    * first select the airport can be reached directly from YEG **
    **************************************************************/
  	char *sql_stmt_q8 = "create table reachable as select p.* from \
                        routes r, airports p where r.Source_airport = 'YEG' \
                        and r.Destination_airport_ID = p.Airport_ID";

  	rc = sqlite3_prepare_v2(db, sql_stmt_q8, -1, &stmt, 0);

    if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed1: %s\n", sqlite3_errmsg(db));
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

    /**********************************************************************
    * preparation for the while loop, which insert the new arrivals      **
    * into the table before, also count the number of reachable airports **
    ***********************************************************************/
    char *sql_updt = "insert into reachable select distinct p.* \
                    from routes r, reachable reach, airports p \
                    where reach.Airport_ID = r.Source_airport_ID \
                    and r.Destination_airport_ID = p.Airport_ID";

    char *sql_count = "select count(distinct Airport_ID) from reachable";

    /********************************************************
    * the idea of this while loop comes from Zhaorui Chen, **
    * later discussed with Hang Zhu                        **
    *********************************************************/ 
    int past=-1, current=0;  
    while(past != current) {

        rc = sqlite3_prepare_v2(db, sql_updt, -1, &stmt, 0);
        if (rc != SQLITE_OK) {  
            fprintf(stderr, "Preparation failed2: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
            fprintf(stderr, "Update failed1: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;     
        }
        //sqlite3_finalize(stmt); //always finalize a statement

        rc = sqlite3_prepare_v2(db, sql_count, -1, &stmt, 0);
        if (rc != SQLITE_OK) {  
            fprintf(stderr, "Preparation failed3: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }

        
        past = current;
        while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            current = atoi((const char *)sqlite3_column_text(stmt, 0));
        } 

        //printf("%d %d\n", past,current);
        sqlite3_finalize(stmt); //always finalize a statement

    } // end of while loop

    /***************************************************
    * finally select airports with commercial flights **
    * cannot be reached from YEG                      **
    ****************************************************/
    char *sql = "select p.Airport_ID, p.Name, p.City, p.Country from airports p, \
    			routes r, airlines l where l.Callsign != '' \
    			and r.Airline_ID = l.Airline_ID and \
    			r.Source_airport_ID = p.Airport_ID \
                union \
                select p.Airport_ID, p.Name, p.City, p.Country from airports p, \
                routes r, airlines l where l.Callsign != '' \
                and r.Airline_ID = l.Airline_ID and \
                r.Destination_airport_ID = p.Airport_ID \
    			except select reach.Airport_ID, reach.Name, reach.City, \
                reach.Country from reachable reach";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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
    char *drop = "drop table reachable";
    rc = sqlite3_exec(db, drop, 0, 0, &zErrMsg);  


    sqlite3_close(db);

    return 0;
  
}