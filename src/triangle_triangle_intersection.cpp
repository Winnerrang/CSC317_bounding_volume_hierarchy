#include "triangle_triangle_intersection.h"
#include <vector>
#include <algorithm>
#include <Eigen/Dense>

#define X 0
#define Y 1
#define Z 2

typedef std::pair<double, Eigen::RowVector3d> pair;

void computeDistanceToPlane(
	const Eigen::RowVector3d& A,
	const Eigen::RowVector3d& B,
	const Eigen::RowVector3d& C,
	const Eigen::RowVector3d& n,
	const double d,
	std::vector<pair>& T
);

int computeLineAndProject(
	const Eigen::RowVector3d& N1,
	const Eigen::RowVector3d& N2
);

void findInterval(
	const std::vector<pair>& T,
	const int axis,
	double& t1,
	double& t2
);

bool coplanarCase(
	const Eigen::RowVector3d& A0,
	const Eigen::RowVector3d& A1,
	const Eigen::RowVector3d& A2,
	const Eigen::RowVector3d& B0,
	const Eigen::RowVector3d& B1,
	const Eigen::RowVector3d& B2,
	const Eigen::RowVector3d& n
);

bool isInside(const Eigen::Vector2d& p, const std::vector<Eigen::Vector2d>& T);

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
	
	Eigen::RowVector3d N1, N2;
	std::vector<pair> T1(3), T2(3);


	N1 = ((A1 - A0).cross(A2 - A0)).normalized();
	computeDistanceToPlane(B0, B1, B2, N1, -N1.dot(A0), T2);
	if (T2[0].first > 0 || T2[2].first < 0) return false;

	if (T2[0].first == 0 && T2[2].first == 0) return coplanarCase(A0, A1, A2, B0, B1, B2, N1);


	N2 = ((B1 - B0).cross(B2 - B0)).normalized();
	computeDistanceToPlane(A0, A1, A2, N2, -N2.dot(B0), T1);
	if (T1[0].first > 0 || T1[2].first < 0) return false;

	int axis = computeLineAndProject(N1, N2);

	double t1, t2, t3, t4;
	findInterval(T1, axis, t1, t2);
	findInterval(T2, axis, t3, t4);

	if (t1 <= t4 && t3 <= t2) return true;
	else return false; 
	return false;
  ////////////////////////////////////////////////////////////////////////////
}

static void computeDistanceToPlane(
	const Eigen::RowVector3d& A,
	const Eigen::RowVector3d& B,
	const Eigen::RowVector3d& C,
	const Eigen::RowVector3d& n,
	const double d,
	std::vector<pair>& T
){
	T[0] = pair(n.dot(A) + d, A);
	T[1] = pair(n.dot(B) + d, B);
	T[2] = pair(n.dot(C) + d, C);
	
	std::sort(T.begin(), T.end(), [](pair a, pair b) {
		return a.first < b.first;
	});

}

static int computeLineAndProject(
	const Eigen::RowVector3d& N1,
	const Eigen::RowVector3d& N2
) {
	auto D = (N1.cross(N2)).normalized();
	if (std::abs(D.x()) > std::abs(D.y()) && std::abs(D.x()) > std::abs(D.z())) return X;
	else if (std::abs(D.y()) > std::abs(D.x()) && std::abs(D.y()) > std::abs(D.z())) return Y;
	else return Z;

}

static void findInterval(
	const std::vector<pair>& T,
	const int axis,
	double& t1,
	double& t2
){
	int ref;

	if (T[1].first > 0) ref = 0;
	else if (T[1].first < 0) ref = 2;
	else {
		if (T[0].first != 0) ref = 0;
		else ref = 2;
	}

	Eigen::Vector3d p;
	
	for (int i = 0; i < 3; i++) {
		p(i) = T[i].second(axis);
	}

	int i = (ref + 1) % 3;
	double tempT1 = p(i) + (p(ref) - p(i)) * (T[i].first / (T[i].first - T[ref].first));

	i = (ref + 2) % 3;
	double tempT2 = p(i) + (p(ref) - p(i)) * (T[i].first / (T[i].first - T[ref].first));

	t1 = std::min(tempT1, tempT2);
	t2 = std::max(tempT1, tempT2);

}

static bool coplanarCase(
	const Eigen::RowVector3d& A0,
	const Eigen::RowVector3d& A1,
	const Eigen::RowVector3d& A2,
	const Eigen::RowVector3d& B0,
	const Eigen::RowVector3d& B1,
	const Eigen::RowVector3d& B2,
	const Eigen::RowVector3d& n
) {
	std::vector<Eigen::Vector2d> T1(3), T2(3);

	int drop, u, v;
	double nx(std::abs(n.x())), ny(std::abs(n.y())), nz(std::abs(n.z()));

	if (nx > ny && nx > nz) drop = X;
	else if (ny > nx && ny > nz) drop = Y;
	else drop = Z;

	u = (drop + 1) % 3;
	v = (drop + 2) % 3;

	T1[0] = Eigen::Vector2d(A0(u), A0(v));
	T1[1] = Eigen::Vector2d(A1(u), A1(v));
	T1[2] = Eigen::Vector2d(A2(u), A2(v));

	T2[0] = Eigen::Vector2d(B0(u), B0(v));
	T2[1] = Eigen::Vector2d(B1(u), B1(v));
	T2[2] = Eigen::Vector2d(B2(u), B2(v));

	if (isInside(T1[0], T2)) return true;

	if (isInside(T1[1], T2)) return true;

	if (isInside(T1[2], T2)) return true;

	if (isInside(T2[0], T1)) return true;

	if (isInside(T2[1], T1)) return true;

	if (isInside(T2[2], T1)) return true;

	return false;
}

static bool isInside(const Eigen::Vector2d& p, const std::vector<Eigen::Vector2d>& T) {
	
	std::vector<double> cross(3);
	
	for (int i = 0; i < 3; i++) {
		cross[i] = p(0) * T[i](1) - p(1) * T[i](0);
	}

	std::sort(cross.begin(), cross.end());

	if (cross[0] >= 0 || cross[2] <= 0) return true;
	else return false;
}


