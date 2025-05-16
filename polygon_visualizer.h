#pragma once

#include "polygon.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class PolygonVisualizer {
public:
    PolygonVisualizer(int width = 800, int height = 600) 
        : window_(sf::VideoMode(width, height), "Polygon Visualization") {
        window_.setFramerateLimit(60);
    }

    void addPolygon(const Polygon& polygon, const sf::Color& color = sf::Color::White) {
        sf::ConvexShape shape;
        shape.setPointCount(polygon.size());
        
        // Find bounds of the polygon
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double maxY = std::numeric_limits<double>::lowest();
        
        for (size_t i = 0; i < polygon.size(); ++i) {
            auto point = polygon.getPoint(i);
            minX = std::min(minX, point.get<0>());
            minY = std::min(minY, point.get<1>());
            maxX = std::max(maxX, point.get<0>());
            maxY = std::max(maxY, point.get<1>());
        }
        
        // Calculate scale to fit in window
        double scaleX = (window_.getSize().x * 0.8) / (maxX - minX);
        double scaleY = (window_.getSize().y * 0.8) / (maxY - minY);
        double scale = std::min(scaleX, scaleY);
        
        // Center the polygon
        double centerX = (minX + maxX) / 2;
        double centerY = (minY + maxY) / 2;
        
        // Set points with scaling and centering
        for (size_t i = 0; i < polygon.size(); ++i) {
            auto point = polygon.getPoint(i);
            float x = (point.get<0>() - centerX) * scale + window_.getSize().x / 2;
            float y = (point.get<1>() - centerY) * scale + window_.getSize().y / 2;
            shape.setPoint(i, sf::Vector2f(x, y));
        }
        
        shape.setFillColor(sf::Color(color.r, color.g, color.b, 128)); // Semi-transparent
        shape.setOutlineColor(color);
        shape.setOutlineThickness(2.0f);
        
        shapes_.push_back(shape);
    }

    void run() {
        while (window_.isOpen()) {
            sf::Event event;
            while (window_.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window_.close();
            }

            window_.clear(sf::Color::Black);
            
            for (const auto& shape : shapes_) {
                window_.draw(shape);
            }
            
            window_.display();
        }
    }

private:
    sf::RenderWindow window_;
    std::vector<sf::ConvexShape> shapes_;
}; 