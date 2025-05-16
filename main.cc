#include <iostream>
#include <cmath>
#include "polygon.h"
#include "polygon_visualizer.h"
#include <boost/geometry.hpp>

void printWindingOrder(const Polygon& poly) {
    std::cout << "Polygon has " << poly.size() << " points" << std::endl;
    std::cout << "Area: " << poly.getArea() << std::endl;
    std::cout << "Winding order: " 
              << (poly.getWindingOrder() == Polygon::WindingOrder::CLOCKWISE ? 
                  "Clockwise" : "Counter-clockwise") 
              << std::endl;
}

// Create a regular polygon with n sides
Polygon createRegularPolygon(int sides, double centerX, double centerY, double radius) {
    Polygon poly;
    for (int i = 0; i < sides; ++i) {
        double angle = 2 * M_PI * i / sides;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        poly.addPoint(x, y);
    }
    return poly;
}

// Calculate the apothem (inradius) of a regular polygon
double calculateApothem(int sides, double circumradius) {
    return circumradius * cos(M_PI / sides);
}

int main() {
    try {
        // Create a collection of polygons with different shapes and positions
        std::vector<Polygon> polygons;
        std::vector<sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color::Cyan,
            sf::Color::Magenta,
            sf::Color(255, 165, 0),  // Orange
            sf::Color(128, 0, 128),  // Purple
            sf::Color(0, 128, 0),    // Dark Green
            sf::Color(128, 128, 0)   // Olive
        };

        // Create a large outer octagon
        polygons.push_back(createRegularPolygon(8, 0.0, 0.0, 5.0));

        // Create a smaller hexagon offset to the right
        polygons.push_back(createRegularPolygon(6, 2.0, 0.0, 3.0));

        // Create a pentagon offset to the left
        polygons.push_back(createRegularPolygon(5, -2.0, 0.0, 2.5));

        // Create a square rotated 45 degrees
        Polygon square = createRegularPolygon(4, 0.0, 2.0, 2.0);
        // Create a new polygon for the rotated square
        Polygon rotatedSquare;
        for (size_t i = 0; i < square.size(); ++i) {
            auto p = square.getPoint(i);
            double x = boost::geometry::get<0>(p);
            double y = boost::geometry::get<1>(p);
            double angle = M_PI / 4;  // 45 degrees
            double newX = x * cos(angle) - y * sin(angle);
            double newY = x * sin(angle) + y * cos(angle);
            rotatedSquare.addPoint(newX, newY);
        }
        polygons.push_back(rotatedSquare);

        // Create a triangle at the bottom
        polygons.push_back(createRegularPolygon(3, 0.0, -2.0, 2.0));

        // Create a small circle (approximated with many sides)
        polygons.push_back(createRegularPolygon(32, 1.5, 1.5, 1.0));

        // Create a star shape (approximated with a 10-sided polygon)
        Polygon star = createRegularPolygon(10, -1.5, 1.5, 1.5);
        // Create a new polygon for the star shape
        Polygon modifiedStar;
        for (size_t i = 0; i < star.size(); ++i) {
            auto p = star.getPoint(i);
            double x = boost::geometry::get<0>(p);
            double y = boost::geometry::get<1>(p);
            if (i % 2 == 0) {
                // Make every other point closer to the center
                x *= 0.5;
                y *= 0.5;
            }
            modifiedStar.addPoint(x, y);
        }
        polygons.push_back(modifiedStar);

        // Print information about each polygon
        std::cout << "Polygons information:" << std::endl;
        for (size_t i = 0; i < polygons.size(); ++i) {
            std::cout << "\nPolygon " << (i + 1) << ":" << std::endl;
            printWindingOrder(polygons[i]);
        }

        // Create visualizer with larger window
        PolygonVisualizer visualizer(1600, 1200);

        // Add all original polygons to the visualizer with reduced opacity
        for (size_t i = 0; i < polygons.size(); ++i) {
            sf::Color originalColor = colors[i];
            originalColor.a = 64;  // Make original shapes very transparent
            visualizer.addPolygon(polygons[i], originalColor);
        }

        // Perform boolean operations between various polygons
        std::cout << "\nBoolean operations between polygons:" << std::endl;
        
        // Union of the octagon and hexagon
        auto unionResult = polygons[0].unionWith(polygons[1]);
        std::cout << "Union of octagon and hexagon: " << unionResult.size() << " resulting polygons" << std::endl;
        for (const auto& poly : unionResult) {
            sf::Color unionColor = sf::Color(255, 0, 0);  // Bright red
            unionColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, unionColor);
        }

        // Difference between the octagon and pentagon
        auto difference = polygons[0].differenceWith(polygons[2]);
        std::cout << "Difference between octagon and pentagon: " << difference.size() << " resulting polygons" << std::endl;
        for (const auto& poly : difference) {
            sf::Color diffColor = sf::Color(0, 255, 0);  // Bright green
            diffColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, diffColor);
        }

        // Intersection of the square and circle
        auto intersection = polygons[3].intersectWith(polygons[5]);
        std::cout << "Intersection of square and circle: " << intersection.size() << " resulting polygons" << std::endl;
        for (const auto& poly : intersection) {
            sf::Color interColor = sf::Color(0, 0, 255);  // Bright blue
            interColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, interColor);
        }

        // Add additional boolean operations for more interesting results
        // Union of the star and triangle
        auto starTriangleUnion = polygons[7].unionWith(polygons[4]);
        std::cout << "Union of star and triangle: " << starTriangleUnion.size() << " resulting polygons" << std::endl;
        for (const auto& poly : starTriangleUnion) {
            sf::Color unionColor = sf::Color(255, 165, 0);  // Orange
            unionColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, unionColor);
        }

        // Difference between the hexagon and circle
        auto hexCircleDiff = polygons[1].differenceWith(polygons[5]);
        std::cout << "Difference between hexagon and circle: " << hexCircleDiff.size() << " resulting polygons" << std::endl;
        for (const auto& poly : hexCircleDiff) {
            sf::Color diffColor = sf::Color(128, 0, 128);  // Purple
            diffColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, diffColor);
        }

        // Intersection of the pentagon and square
        auto pentSquareInter = polygons[2].intersectWith(polygons[3]);
        std::cout << "Intersection of pentagon and square: " << pentSquareInter.size() << " resulting polygons" << std::endl;
        for (const auto& poly : pentSquareInter) {
            sf::Color interColor = sf::Color(0, 255, 255);  // Cyan
            interColor.a = 192;  // More opaque
            visualizer.addPolygon(poly, interColor);
        }

        // Run the visualization
        visualizer.run();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
