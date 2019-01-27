#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <iterator>
#include <limits>
#include <typeinfo>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <cstddef>
#include <cassert>

#define PI 3.141
const double inf {-1.0};

struct Point
{
    Point() { z = 0.0; }
    Point(double inX, double inY) : x {inX}, y {inY}, z {0.0} {}
    double x;
    double y;
    double z;
};

enum class Side : int8_t {Right = -1, InLine, Left};

struct PointsCompare
{
    bool operator() (const Point &lhs, const Point &rhs)
    {
        if(lhs.x == rhs.x)
        {
            return lhs.y < rhs.y;
        }
        return lhs.x < rhs.x;
    }
};

struct PointsCompareDescending
{
    bool operator() (const Point &lhs, const Point &rhs)
    {
        if(lhs.x == rhs.x)
        {
            return lhs.y < rhs.y;
        }
        return lhs.x > rhs.x;
    }
};

namespace ch
{
class ConvexHull
{

    public:
        ConvexHull() = delete;
        explicit ConvexHull(std::vector<Point> xyPoints);

        std::map<std::string, Point> &PolygonField();
        std::vector<Point> &PolygonFieldPoints();
        std::vector<Point> &Hull();
        std::vector<Point> &Hull_t();
        std::vector<Point> &OuterFieldPoints();

    protected:
        friend std::ostream &operator<<(std::ostream os, const Point &xy);
        friend Point &operator-(Point &a,
                                Point &b);
        friend bool operator==(const Point &leftXY, 
                               const Point &rightXY);
        friend bool operator!=(const Point &leftXY, 
                               const Point &rightXY);

    private:
        std::vector<Point> fieldPoints;
        std::map<std::string, Point> innerField;
        std::vector<Point> innerFieldPoints;
        std::vector<Point> hull;
        std::set<Point, PointsCompare> hullLeft;
        std::set<Point, PointsCompareDescending> hullRight;
        std::set<Point, PointsCompare> hullUpperLeft;
        std::set<Point, PointsCompare> hullUpperRight;
        std::set<Point, PointsCompareDescending> hullLowerRight;
        std::set<Point, PointsCompareDescending> hullLowerLeft;
        
        void BuildConvexHull();
        void BuildConvexHull_t();
        void QuickHull(const Point &linePoint1, 
                       const Point &linePoint2, 
                       Side side);
        void QuickHull_t(const Point &linePoint1, 
                       const Point &linePoint2, 
                       Side side,
                       std::string section);
};
}

bool operator==(const Point &leftXY, const Point &rightXY);
bool operator!=(const Point &leftXY, const Point &rightXY);

inline int CrossProduct(const Point &p0, const Point &mid, const Point &p1);
inline float YIntercept(const Point &high, const Point &low, const Point &p);
bool Intersecting(const Point &p1, 
                  const Point &p2, 
                  const Point &q1, 
                  const Point &q2);
void AklToussaint(std::vector<Point> &fP, 
                  std::map<std::string, Point> &pointsMinMax,
                  std::vector<Point> &pFp);
void BuildInnerField(const std::vector<Point> &fP, std::vector<Point> &pF);
double DistanceFromLine (const Point &lp1, const Point &lp2, const Point &p);
