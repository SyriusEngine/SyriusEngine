#pragma once

class MoveComponent{
public:
    MoveComponent();

    MoveComponent(float x, float y);

    ~MoveComponent();

    void move(float x, float y);

    [[nodiscard]] float getX() const;

    [[nodiscard]] float getY() const;

private:
    float m_X;
    float m_Y;
};

void moveComponents(MoveComponent& moveComponent, float x, float y);

class HealthComponent{
public:
    HealthComponent();

    explicit HealthComponent(int health);

    ~HealthComponent();

    void damage(int damage);

    [[nodiscard]] int getHealth() const;

private:
    int m_Health;
};