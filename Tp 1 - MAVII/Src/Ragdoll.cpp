#include "Ragdoll.h"


Ragdoll::Ragdoll(b2World* world, b2Vec2 posicion, float angulo, b2Vec2 fuerzaInicial) {
    // Creo Bodys
    b2Body* torso = Box2DHelper::CreateRectangularDynamicBody(world, 2.5f, 4.0f, 0.2f, 0.2f, 0.0f);
    b2Body* cabeza = Box2DHelper::CreateRectangularDynamicBody(world, 1.5f, 1.5f, 0.3f, 0.2f, 0.0f);
    b2Body* brazoIzq = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 3.0f, 0.3f, 0.2f, 0.0f);
    b2Body* brazoDer = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 3.0f, 0.3f, 0.2f, 0.0f);
    b2Body* piernaIzq = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 5.0f, 0.3f, 0.2f, 0.0f);
    b2Body* piernaDer = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 5.0f, 0.3f, 0.2f, 0.0f);

    // Partes relativas al torso
    torso->SetTransform(posicion, angulo);
    cabeza->SetTransform(RotarYDesplazar(posicion, b2Vec2(0, -2.75f), angulo), angulo);
    brazoIzq->SetTransform(RotarYDesplazar(posicion, b2Vec2(-1.75f, -0.5f), angulo), angulo);
    brazoDer->SetTransform(RotarYDesplazar(posicion, b2Vec2(1.75f, -0.5f), angulo), angulo);
    piernaIzq->SetTransform(RotarYDesplazar(posicion, b2Vec2(-1.0f, 4.5f), angulo), angulo);
    piernaDer->SetTransform(RotarYDesplazar(posicion, b2Vec2(1.0f, 4.5f), angulo), angulo);

    // Conectar articulaciones
    ConectarDistance(world, cabeza, torso, b2Vec2(0, 0.75f), b2Vec2(0, -2.0f), angulo);
    ConectarDistance(world, brazoIzq, torso, b2Vec2(0, -1.5f), b2Vec2(-1.25f, -2.0f), angulo);
    ConectarDistance(world, brazoDer, torso, b2Vec2(0, -1.5f), b2Vec2(1.25f, -2.0f), angulo);
    ConectarDistance(world, piernaIzq, torso, b2Vec2(0.0f, -2.5f), b2Vec2(-1.0f, 2.0f), angulo);
    ConectarDistance(world, piernaDer, torso, b2Vec2(0.0f, -2.5f), b2Vec2(1.0f, 2.0f), angulo);

    // Aplicar fuerza
    torso->ApplyLinearImpulseToCenter(fuerzaInicial, true);

    partes = { cabeza, torso, brazoIzq, brazoDer, piernaIzq, piernaDer };
}

b2Vec2 Ragdoll::RotarYDesplazar(b2Vec2 base, b2Vec2 offset, float angulo) {
    float cosA = cos(angulo);
    float sinA = sin(angulo);
    b2Vec2 rotado(
        offset.x * cosA - offset.y * sinA,
        offset.x * sinA + offset.y * cosA
    );
    return base + rotado;
}
void Ragdoll::ConectarDistance(b2World* world, b2Body* bodyA, b2Body* bodyB, b2Vec2 offsetA, b2Vec2 offsetB, float angulo) {
    b2Vec2 anchorA = RotarYDesplazar(bodyA->GetWorldCenter(), offsetA, angulo);
    b2Vec2 anchorB = RotarYDesplazar(bodyB->GetWorldCenter(), offsetB, angulo);

    b2DistanceJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.bodyB = bodyB;
    jointDef.localAnchorA = bodyA->GetLocalPoint(anchorA);
    jointDef.localAnchorB = bodyB->GetLocalPoint(anchorB);
    jointDef.length = (anchorA - anchorB).Length();
    jointDef.collideConnected = true;

    b2LinearStiffness(jointDef.stiffness, jointDef.damping, 3.0f, 0.7f, bodyA, bodyB);

    world->CreateJoint(&jointDef);
}

const std::vector<b2Body*>& Ragdoll::getPartes() const {
    return partes;
}