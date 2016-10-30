#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define MIN(a,b) ((a) <= (b) ? a : b)
#define MAX(a,b) ((a) >= (b) ? a : b) 

struct Node {
  int node_index;
  int count;
  double mindist;
  double minmaxdist;
  double minX;
  double minY;
  double maxX;
  double maxY;
};

struct Rect {
  int id;
  double minX;
  double minY;
  double maxX;
  double maxY;
  double dist;
};

struct Point {
  double x;
  double y;
};

double square (double a) {
  return a * a;
}

// calculate the minimal distance from the point to the mbr
double minDist (struct Node node, struct Point p) {
  double mindist;
  // if the point is inside the mbr 
  if(p.x >= node.minX && p.x <= node.maxX && p.y >= node.minY && p.y <= node.maxY) {
    mindist = 0;
  }
  // if the point is outside mbr
  else {
    // if the point is within range 
    if(p.y < node.maxY && p.y > node.minY) {
      if(p.x > node.maxX) {
        mindist = square(p.x - node.maxX);
      }
      else if(p.x < node.minX) {
        mindist = square(node.minX - p.x);
      }
    } 
    else if(p.x > node.minX && p.x < node.maxX) {
      if(p.y > node.maxY) {
        mindist = square(p.y - node.maxY);
      }
      else if(p.y < node.minY) {
        mindist = square(node.minY - p.y);
      }
      //mindist = MIN((p.y-node.maxY)*(p.y-node.maxY), (p.y-node.minY)*(p.y-node.minY));
    }
    else {
      if(p.x > node.maxX) {
        mindist  = MIN(square(p.y-node.maxY)+square(p.x-node.maxX), square(p.y-node.minY)+square(p.x-node.maxX));
      }
      else if(p.x < node.minX) {
        mindist = MIN(square(p.y-node.maxY)+square(p.x-node.minX), square(p.y-node.minY)+square(p.x-node.minX));
      }
    }
  }
  return mindist;
}

/*
double minMaxDist(struct Node node, struct Point p) {
  double minMaxDist;
  if(p.x > node.maxX) {
    if(p.y > node.minY && p.y < node.maxY) {
      minMaxDist = MAX(square(p.y-node.maxY)+square(p.x-node.maxX), square(p.y-node.minY)+square(p.x-node.maxX));
      printf("case1\n");
    }
    else {
      if(p.y > node.maxY) {
        minMaxDist = MIN(square(p.y-node.minY)+square(p.x-node.maxX), square(p.y-node.maxY)+square(p.x-node.minX));
        printf("case2\n");
      }
      else if(p.y < node.minY) {
        minMaxDist = MIN(square(p.y-node.maxY)+square(p.x-node.maxX), square(p.y-node.minY)+square(p.x-node.minX));
        printf("case2''\n");
      }
    }
  }
  else if(p.x < node.minX) {
    if(p.y > node.minY && p.y < node.maxY) {
      minMaxDist = MAX(square(p.y-node.maxY)+square(p.x-node.minX), square(p.y-node.minY)+square(p.x-node.minX));
      printf("case3\n");
    }
    else {
      if(p.y > node.maxY) {
        minMaxDist = MIN(square(p.y-node.minY)+square(p.x-node.minX), square(p.y-node.maxY)+square(p.x-node.maxX));
        printf("case4\n");
      }
      else if(p.y < node.minY) {
        minMaxDist = MIN(square(p.y-node.maxY)+square(p.x-node.minX), square(p.y-node.minY)+square(p.x-node.maxX));
        printf("case4''\n");
      }
    }
  }
  else {
    if(p.y > node.maxY) {
      minMaxDist = MAX(square(p.y-node.maxY)+square(p.x-node.minX), square(p.y-node.maxY)+square(p.x-node.maxX));
      printf("case5\n");
    }
    else {
      minMaxDist = MAX(square(p.y-node.minY)+square(p.x-node.maxX), square(p.y-node.minY)+square(p.x-node.minX));
      printf("case6\n");
    }
  }


  return minMaxDist;
}
*/
int main(int argc, char ** argv) 
{
  struct Node n1;
  struct Point p1;
  n1.minX = 39.46; n1.minY = 18.54; n1.maxX = 39.82; n1.maxY = 19.01;
  //n1.minX = 59.77; n1.minY = 33.58; n1.maxX = 60.14; n1.maxY = 34.06;
  p1.x = 0; p1.y = 0;
  double mindist;
  double minmaxdist;
  mindist = minDist(n1,p1);
  //minmaxdist = minMaxDist(n1, p1);
  printf("minimial distance from n1 to p1 is %lf\n", mindist);
  //printf("minmax distance from n1 to p1 is %lf\n", minmaxdist);
  return 0;
}