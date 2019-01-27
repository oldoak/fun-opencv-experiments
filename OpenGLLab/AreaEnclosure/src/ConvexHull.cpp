#include "ConvexHull.h"

// helper functions

/// Intersecting determines which points lie within (minX, maxY, maxX, minY).
bool Intersecting
(const Point &p1, 
 const Point &p2, 
 const Point &q1,
 const Point &q2) 
{
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x))
            * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0)
            &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x))
            * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

/// YIntercept determines the y-intercept for a line between two points.
/** Ax + By + C = 0
    This is equivalent to (y1 - y2)x + (x2 - x1)y + (x1.y2 - x2.y1) = 0 */
double YIntercept
(const Point &p,
 const Point &q)
{
    return (p.x * q.y) - (q.x * p.y);
}

/// CrossProduct determines which side of a line a field point is on.
/** Returns positive values, then it is a “left turn”.
    Returns 0, then it is a “no turn”.
    Returns negative values, then it is a “right turn”. */
inline int CrossProduct
(const Point &p0, 
 const Point &mid, 
 const Point &p1)
{
    double result = ((mid.x-p0.x) * (p1.y-p0.y) - (mid.y-p0.y) * (p1.x-p0.x));
    if(1000 * result < 0)
    {
        return -1;
    }
    else if(1000 * result > 0)
    {
        return 1;
    }

    return 0;
}

/// DistanceFromLine calculates the distance from a point to a line.
/** For a point p(ax, by) and a line Ax + By + C
    distance = (A*a + B*b + C)/sqrt(A*A + B*B) */
double DistanceFromLine
(const Point &lp1,
 const Point &lp2,
 const Point &p
)
{
    double A {lp1.y - lp2.y};
    double B {lp2.x - lp1.x};
    double C {YIntercept(lp1, lp2)};

    double denominator {std::sqrt((A * A) + (B * B))};
    double distance {((A * p.x) + (B * p.y) + C) / denominator};
    return distance;
}

/// BuildInnerField returns the extreme points of a quadrilateral for
/// eliminating a large set of points that would not be part of the convex hull.
void BuildInnerField
(const std::vector<Point> &fP,
 std::map<std::string, Point> &mm)
{
    Point minY = fP[0];
    Point maxX = fP[0];
    Point maxY = fP[0];
    Point minX = fP[0];

    for(size_t index {1}; index < fP.size(); ++index)
    {
        Point xy {fP[index]};
        
        if(xy.y < minY.y) { minY = xy; }

        if(xy.x > maxX.x || (xy.x == maxX.x && xy.y < maxX.y)) { maxX = xy; }

        if(xy.y > maxY.y) { maxY = xy; }

        if(xy.x < minX.x || (xy.x == minX.x && xy.y > minX.y)) { minX = xy; }
    }

    mm["minX"] = minX;
    mm["maxY"] = maxY;
    mm["maxX"] = maxX;
    mm["minY"] = minY;
}

/// Akl–Toussaint heuristic elminates a large set of points that would not
/// be part of the convex hull and therefore reducing the number of points
/// that need to be tested for convex hull inclusion.
void AklToussaint
(std::vector<Point> &fP, 
 std::map<std::string, Point> &pointsMinMax,
 std::vector<Point> &pFp)
{
    if(fP.size() <= 3)
    {
        std::cerr << "Data points sample too small" << std::endl;
        return;
    }

    BuildInnerField(fP, pointsMinMax);
    Point minX {pointsMinMax["minX"]};
    Point maxY {pointsMinMax["maxY"]};
    Point maxX {pointsMinMax["maxX"]};
    Point minY {pointsMinMax["minY"]};

    for(auto it {fP.begin()}; it != fP.end(); )
    {
        Point infinite {inf, it->y};
        int intersectCount {0};
        if(Intersecting(minX, maxY, *it, infinite)) ++intersectCount;
        if(Intersecting(maxY, maxX, *it, infinite)) ++intersectCount;
        if(Intersecting(maxX, minY, *it, infinite)) ++intersectCount;
        if(Intersecting(minY, minX, *it, infinite)) ++intersectCount;

        // anomaly : when inner point lies on same x-line as minX vertex
        if(it->y == minX.y && it->x > minX.x) intersectCount = 1;

        if(*it == minX || *it == maxY || *it == maxX || *it == minY) 
            --intersectCount;

        if(intersectCount % 2 > 0)
        {
            pFp.push_back(*it);
            fP.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// friend methods
std::ostream &operator<<
(std::ostream &os, 
 const Point &xy)
{
    os << xy.x << " " << xy.y;

    return os;
}

bool operator==
(const Point &leftXY, 
 const Point &rightXY)
{
    return (leftXY.x == rightXY.x && leftXY.y == rightXY.y);
}

bool operator!=
(const Point &leftXY, 
 const Point &rightXY)
{
    return !(leftXY == rightXY);
}

// class methods
/// Constructor eliminates a large sub-set of points using Akl–Toussaint 
/// heuristic.
ch::ConvexHull::ConvexHull
(std::vector<Point> xyPoints)
: fieldPoints {std::move(xyPoints)}
{
    try
    {
        AklToussaint(fieldPoints, innerField, innerFieldPoints);
        BuildConvexHull_t();
    }
    catch(int e)
    {
        std::cout << e << std::endl;
    }
}

/// Quickhull is a recursive call to build set of points for convex hull.
void
ch::ConvexHull::QuickHull
(const Point &linePoint1,
 const Point &linePoint2,
 Side side
)
{
    double pointDistance {0.0};
    Point hullPoint;
    bool theTruth {false};
    for(const Point &p:fieldPoints)
    {
        if(CrossProduct(linePoint1, linePoint2, p) == 1)
        {
            theTruth = true;
            if(DistanceFromLine(linePoint1, linePoint2, p) > pointDistance)
            {
                pointDistance = DistanceFromLine(linePoint1, linePoint2, p);
                hullPoint = p;
            }
        }
    }

    if(theTruth == false) { return; }

    if(side == Side::Left)
    {
        hullLeft.insert(hullPoint);
    }
    else if(side == Side::Right)
    {
        hullRight.insert(hullPoint);
    }

    QuickHull(linePoint1, hullPoint, side);
    QuickHull(hullPoint, linePoint2, side);
}

void
ch::ConvexHull::QuickHull_t
(const Point &linePoint1,
 const Point &linePoint2,
 Side side,
 std::string section
)
{
    double pointDistance {0.0};
    Point hullPoint;
    bool theTruth {false};
    for(const Point &p:fieldPoints)
    {
        if(CrossProduct(linePoint1, linePoint2, p) == 1)
        {
            theTruth = true;
            if(DistanceFromLine(linePoint1, linePoint2, p) > pointDistance)
            {
                pointDistance = DistanceFromLine(linePoint1, linePoint2, p);
                hullPoint = p;
            }
        }
    }

    if(theTruth == false) { return; }

    if(side == Side::Left)
    {
        if(section.compare("ul") != 0)
        {
            hullUpperLeft.insert(hullPoint);
        }
        if(section.compare("ur") != 0)
        {
            hullUpperRight.insert(hullPoint);
        }
    }
    else if(side == Side::Right)
    {
        if(section.compare("lr") != 0)
        {
            hullLowerRight.insert(hullPoint);
        }
        if(section.compare("ll") != 0)
        {
            hullLowerLeft.insert(hullPoint);
        }
    }

    QuickHull_t(linePoint1, hullPoint, side, section);
    QuickHull_t(hullPoint, linePoint2, side, section);
}

/// BuildConvexHull sets the initial values for the convex hull set and
/// initiates the call to QuickHull.
void
ch::ConvexHull::BuildConvexHull()
{
    if(fieldPoints.size() <= 3) { throw -1; }

    // add base points for convex hull
    hullLeft.insert(innerField["minX"]);
    hullRight.insert(innerField["maxX"]);
    hullRight.insert(innerField["minX"]); // complete convex hull enclosure

    QuickHull(innerField["minX"], innerField["maxX"], Side::Left);
    QuickHull(innerField["maxX"], innerField["minX"], Side::Right);
}

/// BuildConvexHull sets the initial values for the convex hull set and
/// initiates the call to QuickHull.
void
ch::ConvexHull::BuildConvexHull_t()
{
    if(fieldPoints.size() <= 3) { throw -1; }

    // add base points for convex hull
    hullUpperLeft.insert(innerField["minX"]);
    hullUpperLeft.insert(innerField["maxY"]);
    
    //hullUpperRight.insert(innerField["maxY"]);
    hullUpperRight.insert(innerField["maxX"]);
    
    hullLowerRight.insert(innerField["maxX"]);
    hullLowerRight.insert(innerField["minY"]);
    
    hullLowerLeft.insert(innerField["minY"]);
    hullLowerLeft.insert(innerField["minX"]);

    QuickHull_t(innerField["minX"], innerField["maxY"], Side::Left, "ul");
    QuickHull_t(innerField["maxY"], innerField["maxX"], Side::Left, "ur");
    QuickHull_t(innerField["maxX"], innerField["minY"], Side::Right, "lr");
    QuickHull_t(innerField["minY"], innerField["minX"], Side::Right, "ll");
}

std::map<std::string, Point> 
&ch::ConvexHull::PolygonField()
{
    return innerField;
}

std::vector<Point> 
&ch::ConvexHull::PolygonFieldPoints()
{
    return innerFieldPoints;
}

std::vector<Point> 
&ch::ConvexHull::OuterFieldPoints()
{
    return fieldPoints;
}

std::vector<Point> 
&ch::ConvexHull::Hull()
{
    hull.insert(hull.end(), hullLeft.begin(), hullLeft.end());
    hull.insert(hull.end(), hullRight.begin(), hullRight.end());
    return hull;
}

std::vector<Point> 
&ch::ConvexHull::Hull_t()
{
    hull.insert(hull.end(), hullUpperLeft.begin(), hullUpperLeft.end());
    hull.insert(hull.end(), hullUpperRight.begin(), hullUpperRight.end());
    hull.insert(hull.end(), hullLowerRight.begin(), hullLowerRight.end());
    hull.insert(hull.end(), hullLowerLeft.begin(), hullLowerLeft.end());
    return hull;
}
