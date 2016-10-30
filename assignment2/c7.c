#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define MIN(a,b) ((a) > (b) ? a : b)
#define MAX(a,b) ((a) < (b) ? a : b) 

struct Node {
  int node_index;
  int count;
  int mindist;
  int minmaxdist;
  double minX;
  double minY;
  double maxX;
  double maxY;
};

struct Point {
  double x;
  double y;
};


double sqrt(double a) {
  return a * a;
}

// calculate the minimal distance from the point to the mbr
double MINDIST (struct Node node, struct point p) {
  double mindist;
  // if the point is inside the mbr 
  if(p.x >= node.minX && p.x <= node.maxX && p.y >= node.minY && p.y <= maxY) {
    dist = 0;
  }
  // if the point is outside mbr
  else {
    // if the point is within range 
    if(p.y < maxY && p.y > minY) {
      if(p.x > node.maxX) {
        mindist = sqrt(p.x - node.maxX);
      }
      else(p.x < node.minX) {
        mindist = sqrt(node.minX - p.x);
      }
    } 
    if(poi.x > minX && poi.x < maxX) {
      if(p.y > node.maxY) {
        mindist = sqrt(p.y - node.maxY);
      }
      else(p.y < node.minY) {
        mindist = sqrt(node.minY - p.Y);
      }
      //mindist = MIN((p.y-node.maxY)*(p.y-node.maxY), (p.y-node.minY)*(p.y-node.minY));
    }
    else {
      if(p.x > maxX) {
        mindist  = MIN(sqrt(p.y-node.maxY)+sqrt(p.x-node.maxX), sqrt(p.y-node.minY)+sqrt(p.x-node.minX));
      }
      if(p.x < minX) {
        mindist = MIN(sqrt(p.y-node.maxY)+sqrt(p.x-node.minX), sqrt(p.y-node.minY)+sqrt(p.x-node.minX));
      }
    }
  }


double MINMAXDIST (struct Node node, srtuct point p) {
  double minmaxdist;
  if(p.y < (node.minY+node.maxY)/2) {
    if(p.y > node.minY)
    minmaxdist = (p.x - maxX)*(p.x - maxX) + (p.y - maxY)*(p.y - maxY);
  }
  if(p.y )
}


void NearestNeighborSearch(struct Node node, struct Point p, struct NearestN nearest) {

  struct Node newNode;
  char * branchList;
  int dist, last, i;

  // At leaf level - compute distance to actual object
  if(node.type == LEAF) {
    for(i = 0; i < Node.count; i++) {
      dist = objectDIST(Point, Node.branch[i].rect);
      if(dist < Nearest.dist) {
        Nearest.dist = dist;
        Nearest.rect = Node.branch[i].rect;
      }
    }
  }

  // Non-leaf level - order, prune and visit nodes
  else {
    // Generate Active Branch List
    genBranchList(Point, Node, branchList);

    // Sort ALB based on ordering metric values
    sortBranchList(branchList);

    // Perform Downward Pruning
    // (may discard all branched)
    last = pruneBranchList(Node, Point, Nearest, branchList);

    // Iterate through the Active Branch List
    for(i = 0; i <= last; i++) {
      newNode = Node.branch[branchList];

      // Recursively visit child nodes
      NearestNeighborSearch(newNode, Point, Nearest);

      // Perform Upward Pruning
      last = pruneBranchList(Node, Point, Nearest, branchList);
    
    }

  }
} 