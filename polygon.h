#pragma once

#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/union.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/difference.hpp>

namespace bg = boost::geometry;

class Polygon {
public:
    enum class WindingOrder {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };

    // Define point type using double coordinates
    using Point = bg::model::point<double, 2, bg::cs::cartesian>;
    using BoostPolygon = bg::model::polygon<Point>;
    using MultiPolygon = bg::model::multi_polygon<BoostPolygon>;

    Polygon() = default;
    
    // Add a point to the polygon
    void addPoint(double x, double y) {
        points_.push_back(Point(x, y));
    }

    // Get the winding order of the polygon
    WindingOrder getWindingOrder() const {
        if (points_.size() < 3) {
            throw std::runtime_error("Polygon must have at least 3 points");
        }

        double sum = 0.0;
        for (size_t i = 0; i < points_.size(); ++i) {
            const Point& p1 = points_[i];
            const Point& p2 = points_[(i + 1) % points_.size()];
            sum += (p2.get<0>() - p1.get<0>()) * (p2.get<1>() + p1.get<1>());
        }

        return sum > 0 ? WindingOrder::CLOCKWISE : WindingOrder::COUNTER_CLOCKWISE;
    }

    // Get the area of the polygon
    double getArea() const {
        if (points_.size() < 3) {
            return 0.0;
        }

        BoostPolygon poly;
        for (const auto& point : points_) {
            bg::append(poly, point);
        }
        return bg::area(poly);
    }

    // Get the number of points in the polygon
    size_t size() const {
        return points_.size();
    }

    // Get a point at a specific index
    Point getPoint(size_t index) const {
        if (index >= points_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return points_[index];
    }

    // Convert to Boost polygon
    BoostPolygon toBoostPolygon() const {
        BoostPolygon poly;
        for (const auto& point : points_) {
            bg::append(poly, point);
        }
        return poly;
    }

    // Create from Boost polygon
    static Polygon fromBoostPolygon(const BoostPolygon& poly) {
        Polygon result;
        for (const auto& point : poly.outer()) {
            result.addPoint(point.get<0>(), point.get<1>());
        }
        return result;
    }

    // Union operation
    std::vector<Polygon> unionWith(const Polygon& other) const {
        MultiPolygon result;
        bg::union_(toBoostPolygon(), other.toBoostPolygon(), result);
        
        std::vector<Polygon> polygons;
        for (const auto& poly : result) {
            polygons.push_back(fromBoostPolygon(poly));
        }
        return polygons;
    }

    // Intersection operation
    std::vector<Polygon> intersectWith(const Polygon& other) const {
        MultiPolygon result;
        bg::intersection(toBoostPolygon(), other.toBoostPolygon(), result);
        
        std::vector<Polygon> polygons;
        for (const auto& poly : result) {
            polygons.push_back(fromBoostPolygon(poly));
        }
        return polygons;
    }

    // Difference operation
    std::vector<Polygon> differenceWith(const Polygon& other) const {
        MultiPolygon result;
        bg::difference(toBoostPolygon(), other.toBoostPolygon(), result);
        
        std::vector<Polygon> polygons;
        for (const auto& poly : result) {
            polygons.push_back(fromBoostPolygon(poly));
        }
        return polygons;
    }

private:
    std::vector<Point> points_;
}; 