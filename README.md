# Patricks_3rdPersonShooter_Simple

# Project Steps:
1. Create Components (Parent Classes: BasePawn)
2. Create Sub-Components (Child Classes: Tank, Tower, Projectile)
3. User Input (Moving the Actor, Firing)
4. Actions and Events (Hit Events, Health Component, Apply Damage)
5. Game Rules, Game Mode and Game Controller (Game Cycle: Start > Death > Winning, Loosing > End)
6. Special Effects (Sounds, Particles)


-> #include are not described here and must be added to each respective component when needed. Refer to the code documents.

# 1: Create Components

## 1.1: BasePawn C++ class
Create a new C++ class "BasePawn" inheriting from the Pawn class that will be the base class for both our player and our enemies.

## 2.2: Blueprints and Add Components
Create a new blueprint based on BasePawn.cpp "BP_PawnCowPlayer". 
Open it and add components to it: a collision Capsule, a Static Mesh (select which mesh you want to use for this actor) and a projectile spawn point

Make the Capsule the RootComponent by dragging it on to of DefaultSceneRoot




