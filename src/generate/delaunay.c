#include <math.h>
#include <stdlib.h>

#include "../tsp.h"
#include "../graph.h"
#include "delaunay_impl.h"

// Generate the graph from a Delaunay triangulation of the cities


void tsp_generate_delaunay(tsp_graph *g, tsp_problem *p){

	tspg_init(g, p->size);


	// Gather all the points
	del_point2d_t *points = (del_point2d_t *) malloc(sizeof(del_point2d_t) * p->size);
	for(int i = 0; i < p->size; i++){
		points[i].x = p->nodes[i].x;
		points[i].y = p->nodes[i].y;
	}



	// Triangulate
	delaunay2d_t* res = delaunay2d_from(points, p->size);


	// Convert triangulation to a graph
	int offset = 0;
	for(int i = 0; i < res->num_faces; i++){
		int num_verts = res->faces[offset];
		offset++;

		for(int j = 0; j < num_verts; j++){
			int p0 = res->faces[offset + j];
			int p1 = res->faces[offset + (j+1) % num_verts];


			// TODO: Only do this if the edge wasn't created yet
			tsp_node *n = &p->nodes[p0];
			tsp_node *m = &p->nodes[p1];

			*tspg_edge(g, p0, p1) = hypot(n->x - m->x, n->y - m->y); // The distance between the points


			//pf[j] = QPointF(points[p0].x, points[p0].y);
			//painter.drawLine(points[p0].x, points[p0].y, points[p1].x, points[p1].y);
		}

		//int c	= (rand() % 256 + rand() % 256 + rand() % 256) / 3;
		//painter.setBrush(QBrush(QColor(c, c, c)));//QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
		//painter.drawPolygon(pf, num_verts);
		offset += num_verts;
	}




	delaunay2d_release(res);
}
