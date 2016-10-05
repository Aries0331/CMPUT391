// prints the list of types of airplanes capable of operating flights over 10,000 Km in length
// may have some difference at distance of two airports because choose pi as 3.14
// fail to separate the line 

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) 
{
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the select statement

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

    sqlite3_enable_load_extension(db,1);
    sqlite3_load_extension(db,"libsqlitefunctions",0,0);

  	/*******************************************************************
    * calculate the distance of each routes with commercial flights   **
    * part of the formular                                            **
    * cites from http://www.movable-type.co.uk/scripts/latlong.html   **                                         
    ********************************************************************/
  	char *sql_stmt = "create table length as select r.Source_airport, r.Destination_airport, \
                        (sin((p2.Latitude-p1.Latitude)/2*3.14/180)*sin((p2.Latitude-p1.Latitude)/2*3.14/180) \
                        +cos(p1.Latitude*3.14/180)*cos(p2.Latitude*3.14/180)*sin((p2.Longitude-p1.Longitude)/2*3.14/180)* \
                        sin((p2.Longitude-p1.Longitude)/2*3.14/180)) as a, r.Equipments \
                        from airports p1, airports p2, routes r, airlines l \
                        where r.Source_airport_ID = p1.Airport_ID and \
                        r.Destination_airport_ID = p2.Airport_ID and \
                        r.Airline_ID = l.Airline_ID";

    rc = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

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


  /***********************************************************
   *  calculate the complete distance from two airports and **
   *  choose the distance >= 10000km                        **
   ***********************************************************/
 char *stmt2 = "select l.Equipments, \
                  6371*2*atan2(sqrt(a),sqrt(1-a)) \
                  as distance from length l \
                  where distance >= 10000 group by distance \
                  order by distance desc";
      rc = sqlite3_prepare_v2(db, stmt2, -1, &stmt, 0);

  if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed2: %s\n", sqlite3_errmsg(db));
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
    char *drop = "drop table length";
    rc = sqlite3_exec(db, drop, 0, 0, &zErrMsg);  
    sqlite3_close(db);

    return 0;

}