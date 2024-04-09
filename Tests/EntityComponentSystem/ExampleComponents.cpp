#include "ExampleComponents.hpp"

MoveComponent::MoveComponent():
m_X(0.0f),
m_Y(0.0f){

}

MoveComponent::MoveComponent(float x, float y):
m_X(x),
m_Y(y){

}

void MoveComponent::move(float x, float y) {
    m_X += x;
    m_Y += y;
}

float MoveComponent::getX() const {
    return m_X;
}

float MoveComponent::getY() const {
    return m_Y;
}

MoveComponent::~MoveComponent() = default;

HealthComponent::HealthComponent():
m_Health(100){

}

HealthComponent::HealthComponent(int health):
m_Health(health){

}

HealthComponent::~HealthComponent() = default;

void HealthComponent::damage(int damage) {
    m_Health -= damage;
}

int HealthComponent::getHealth() const {
    return m_Health;
}
