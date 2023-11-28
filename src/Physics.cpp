#include "Physics.hpp"
#include "Components.hpp"

Vec2 doOverlap(const Vec2& aPos, const Vec2& bPos, const Vec2& aBoxSize, const Vec2& bBoxSize)
{
    // Calculate distance between entities
    float dX = abs(aPos.x - bPos.x);
    float dY = abs(aPos.y - bPos.y);

    // Check if entities are too far apart to overlap
    if (dX > aBoxSize.x + bBoxSize.x && dY > aBoxSize.y + bBoxSize.y)
    {
        return Vec2(0, 0);
    }

    // Calculate overlap
    float oX = aBoxSize.x + bBoxSize.x - dX;
    float oY = aBoxSize.y + bBoxSize.y - dY;

    return Vec2(oX, oY);
}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // Check for bounding box components
    if (!(a->hasComponent<CBoundingBox>() && b->hasComponent<CBoundingBox>()))
    {
        return Vec2(0, 0);
    }

    Vec2 aPos = a->getComponent<CTransform>().pos;
    Vec2 bPos = b->getComponent<CTransform>().pos;
    auto& aBox = a->getComponent<CBoundingBox>();
    auto& bBox = b->getComponent<CBoundingBox>();

    return doOverlap(aPos, bPos, aBox.halfSize, bBox.halfSize);
  }

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // Check for bounding box components
    if (!(a->hasComponent<CBoundingBox>() && b->hasComponent<CBoundingBox>()))
    {
        return Vec2(0, 0);
    }

    Vec2 aPos = a->getComponent<CTransform>().prevPos;
    Vec2 bPos = b->getComponent<CTransform>().prevPos;
    auto& aBox = a->getComponent<CBoundingBox>();
    auto& bBox = b->getComponent<CBoundingBox>();

    return doOverlap(aPos, bPos, aBox.halfSize, bBox.halfSize);
}