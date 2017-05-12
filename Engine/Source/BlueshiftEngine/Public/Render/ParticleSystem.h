// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

/*
-------------------------------------------------------------------------------

    Particle System

-------------------------------------------------------------------------------
*/

#include "Core/TimedMinMaxVar.h"
#include "Material.h"

BE_NAMESPACE_BEGIN

class ParticleMesh;

// Definition of the each particles
struct Particle {
    enum { MaxTrails = 32 };

    struct Trail {
        Vec3                    position;
        float                   size;
        float                   angle;
        float                   aspectRatio;
        Color4                  color;
    };

    bool                        generated;
    bool                        alive;
    int                         cycle;

    Vec3                        direction;
    Mat4                        worldMatrix;

    Vec3                        initialPosition;
    float                       initialSpeed;
    float                       initialSize;
    float                       initialAspectRatio;
    float                       initialAngle;
    Color4                      initialColor;
    
    Vec3                        randomForce;            ///< Random seed for force [0, 1]
    float                       randomSpeed;            ///< Random seed for speed over lifetime [0, 1]
    float                       randomSize;             ///< Random seed for size over lifetime [0, 1]
    float                       randomAspectRatio;      ///< Random seed for aspect ratio over lifetime [0, 1]
    float                       randomAngularVelocity;  ///< Random seed for rotation over lifetime [0, 1]

    Trail                       trails[1];
};

class ParticleSystem {
    friend class ParticleMesh;
    friend class ParticleSystemManager;

public:
    enum ModuleBit {
        StandardModuleBit       = 0,
        ShapeModuleBit          = 1,
        CustomPathModuleBit     = 2,
        LTColorModuleBit        = 3,
        LTSpeedModuleBit        = 4,
        LTForceModuleBit        = 5,
        LTRotationModuleBit     = 6,
        LTSizeModuleBit         = 7,
        LTAspectRatioModuleBit  = 8,
        TrailsModuleBit         = 9,
        MaxModules
    };    

    // standard module
    struct StandardModule {
        enum Orientation {
            View,                                       ///< Billboard
            ViewZ,                                      ///< Billboard aligned Z axis
            Aimed,
            AimedZ,
            X,                                          ///< Quad aligned X axis
            Y,                                          ///< Quad aligned Y axis
            Z                                           ///< Quad aligned Z axis
        };

        enum SimulationSpace {
            Local, 
            Global
        };

        void                    Reset();

        int                     count;
        float                   spawnBunching;          ///< 0.0 = all come out at first instant, 1.0 = evenly spaced over life time
        float                   lifeTime;               ///< Total seconds of life for each particles
        float                   deadTime;               ///< Time after lifeTime before re-emit
        bool                    looping;
        bool                    prewarm;
        int                     maxCycles;
        
        float                   simulationSpeed;
        SimulationSpace         simulationSpace;

        Material *              material;
        Orientation             orientation;

        MinMaxVar               startDelay;             ///< Delay from start for the first particle to emit 
        Color4                  startColor;             ///< Start color (RGBA) of particles
        TimedMinMaxVar          startSpeed;             ///< Start speed of particles
        TimedMinMaxVar          startSize;              ///< Start size of particles
        TimedMinMaxVar          startAspectRatio;       ///< Start aspect ratio of particles
        TimedMinMaxVar          startRotation;          ///< Start angle of particles in degrees
        float                   randomizeRotation;      ///< Bias [0, 1] of randomized angle
        float                   gravity;                ///< Can be nagative to float up
    };

    // shape module
    struct ShapeModule {
        enum Shape {
            BoxShape,
            SphereShape,
            CircleShape,
            ConeShape
        };

        void                    Reset();

        Shape                   shape;
        Vec3                    extents;                // used in BoxShape
        float                   radius;                 // used in SphereShape, CircleShape, ConeShape
        float                   thickness;              // used in SphereShape, CircleShape, ConeShape
        float                   angle;                  // used in ConeShape
        float                   randomizeDir;           // [0, 1]
    };

    // custom path module
    struct CustomPathModule {
        enum CustomPath {
            ConePath,
            HelixPath,
            SphericalPath
        };

        void                    Reset();

        CustomPath              customPath;
        float                   radialSpeed;            // used in Cone, Helix, Spherical
        float                   axialSpeed;             // used in Spherical
        float                   innerRadius;
        float                   outerRadius;
    };    
    
    // color over lifetime module
    struct LTColorModule {
        void                    Reset() { targetColor.Set(1, 1, 1, 0); fadeLocation = 0.5f; }

        Color4                  targetColor;
        float                   fadeLocation;
    };

    // speed over lifetime module
    struct LTSpeedModule {
        void                    Reset() { speed.Reset(); }

        TimedMinMaxVar          speed;
    };

    // force module
    struct LTForceModule {
        void                    Reset() { force[0].Reset(); force[1].Reset(); force[2].Reset(); }

        MinMaxVar               force[3];               ///< X, Y, Z
    };

    // rotation over lifetime module
    struct LTRotationModule {
        void                    Reset() { rotation.Reset(); }

        TimedMinMaxVar          rotation;               ///< angular velocity
    };

    // size over lifetime module
    struct LTSizeModule {
        void                    Reset() { size.Reset(); size.constants[1] = CentiToUnit(10.0f); }

        TimedMinMaxVar          size;
    };

    // aspect ratio over lifetime module
    struct LTAspectRatioModule {
        void                    Reset() { aspectRatio.Reset(); aspectRatio.constants[1] = 1.0f; }

        TimedMinMaxVar          aspectRatio;
    };

    // trails module
    struct TrailsModule {
        void                    Reset() { count = 1; length = 0.1f; trailScale = 1.0f, trailCut = true; }

        int                     count;
        float                   length;         // [0.0, 1.0]
        float                   trailScale;
        bool                    trailCut;
    };

    struct Stage {
        void                    Reset();

        int                     TrailCount() const;

        Str                     name;
        bool                    skipRender;     // used for Editor
        int                     moduleFlags;
        StandardModule          standardModule;
        ShapeModule             shapeModule;
        CustomPathModule        customPathModule;
        LTColorModule           colorOverLifetimeModule;
        LTSpeedModule           speedOverLifetimeModule;
        LTForceModule           forceOverLifetimeModule;
        LTRotationModule        rotationOverLifetimeModule;
        LTSizeModule            sizeOverLifetimeModule;
        LTAspectRatioModule     aspectRatioOverLifetimeModule;
        TrailsModule            trailsModule;
    };

    ParticleSystem();
    ~ParticleSystem();

    const char *                GetName() const { return name; }
    const char *                GetHashName() const { return hashName; }

    void                        Purge();

    bool                        Create(const char *text);

    void                        CreateDefaultParticleSystem();

    int                         NumStages() const { return stages.Count(); }
    Stage *                     GetStage(int stageIndex) { return &stages[stageIndex]; }
    void                        AddStage();
    bool                        RemoveStage(int stageIndex);
    void                        SwapStages(int stageIndex0, int stageIndex1);

    bool                        Load(const char *filename);
    bool                        Reload();
    void                        Write(const char *filename);
    const ParticleSystem *      AddRefCount() const { refCount++; return this; }

private:
    bool                        ParseStage(Lexer &lexer, Stage &stage) const;
    bool                        ParseStandardModule(Lexer &lexer, StandardModule &module) const;
    bool                        ParseSimulationSpace(Lexer &lexer, StandardModule::SimulationSpace *simulationSpace) const;
    bool                        ParseOrientation(Lexer &lexer, StandardModule::Orientation *orientation) const;
    bool                        ParseMinMaxVar(Lexer &lexer, MinMaxVar *var) const;
    bool                        ParseTimedVar(Lexer &lexer, TimedVar *var) const;
    bool                        ParseTimedMinMaxVar(Lexer &lexer, TimedMinMaxVar *var) const;
    bool                        ParseTimeWrapMode(Lexer &lexer, Hermite<float>::TimeWrapMode *timeWrapMode) const;
    bool                        ParseShapeModule(Lexer &lexer, ShapeModule &module) const;
    bool                        ParseShape(Lexer &lexer, ShapeModule::Shape *shape) const;
    bool                        ParseCustomPathModule(Lexer &lexer, CustomPathModule &module) const;
    bool                        ParseCustomPath(Lexer &lexer, CustomPathModule::CustomPath *customPath) const;
    bool                        ParseLTForceModule(Lexer &lexer, LTForceModule &module) const;
    bool                        ParseLTColorModule(Lexer &lexer, LTColorModule &module) const;
    bool                        ParseLTSpeedModule(Lexer &lexer, LTSpeedModule &module) const;
    bool                        ParseLTRotationModule(Lexer &lexer, LTRotationModule &module) const;
    bool                        ParseLTSizeModule(Lexer &lexer, LTSizeModule &module) const;
    bool                        ParseLTAspectRatioModule(Lexer &lexer, LTAspectRatioModule &module) const;
    bool                        ParseTrailsModule(Lexer &lexer, TrailsModule &module) const;

    Str                         hashName;
    Str                         name;
    mutable int                 refCount;
    bool                        permanence;

    Array<Stage>                stages;
};

BE_INLINE ParticleSystem::ParticleSystem() {
    refCount = 0;
    permanence = false;
}

BE_INLINE ParticleSystem::~ParticleSystem() {
    Purge();
}

BE_INLINE void ParticleSystem::StandardModule::Reset() {
    lifeTime = 2.0f;
    deadTime = 0.0f;
    looping = true;
    prewarm = true;
    maxCycles = 1;
    count = 32;
    spawnBunching = 1.0f;
    simulationSpeed = 1.0f;
    simulationSpace = SimulationSpace::Local;
    material = materialManager.defaultMaterial;
    orientation = Orientation::View;
    startDelay.Reset();
    startColor.Set(1, 1, 1, 1);
    startSpeed.Reset();
    startSpeed.constants[1] = MeterToUnit(2.0f);
    startSize.Reset();
    startSize.constants[1] = CentiToUnit(10.0f);
    startAspectRatio.Reset();
    startAspectRatio.constants[1] = 1.0f;
    startRotation.Reset();
    randomizeRotation = 1.0f;
    gravity = 0.0f;
}

BE_INLINE void ParticleSystem::ShapeModule::Reset() {
    shape = Shape::ConeShape;
    extents = Vec3::zero;
    radius = MeterToUnit(0.1f);
    thickness = 1.0f;
    angle = 30;
    randomizeDir = 0.0f;
}

BE_INLINE void ParticleSystem::CustomPathModule::Reset() {
    customPath = CustomPath::ConePath;
    radialSpeed = 180;
    axialSpeed = 90;
    innerRadius = MeterToUnit(0.1f);
    outerRadius = MeterToUnit(1.0f);
}

BE_INLINE void ParticleSystem::Stage::Reset() {
    moduleFlags = BIT(StandardModuleBit);
    standardModule.Reset();
    shapeModule.Reset();
    customPathModule.Reset();
    forceOverLifetimeModule.Reset();
    colorOverLifetimeModule.Reset();
    speedOverLifetimeModule.Reset();
    rotationOverLifetimeModule.Reset();
    sizeOverLifetimeModule.Reset();
    aspectRatioOverLifetimeModule.Reset();
    trailsModule.Reset();
}

BE_INLINE int ParticleSystem::Stage::TrailCount() const {
    int trailCount = (moduleFlags & BIT(ParticleSystem::TrailsModuleBit)) ? trailsModule.count : 0;
    return trailCount;
}

class ParticleSystemManager {
public:
    void                        Init();
    void                        Shutdown();

    ParticleSystem *            AllocParticleSystem(const char *name);
    ParticleSystem *            FindParticleSystem(const char *name) const;
    ParticleSystem *            GetParticleSystem(const char *name);

    void                        ReleaseParticleSystem(ParticleSystem *particleSystem, bool immediateDestroy = false);
    void                        DestroyParticleSystem(ParticleSystem *particleSystem);
    void                        DestroyUnusedParticleSystems();

    static ParticleSystem *     defaultParticleSystem;

private:
    StrIHashMap<ParticleSystem *> particleSystemHashMap;
};

extern ParticleSystemManager    particleSystemManager;

BE_NAMESPACE_END