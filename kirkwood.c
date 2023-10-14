#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define G 1.0f
#define M_SUN 1.0f
#define NUM_ASTEROIDS 2500

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float mass;
    float radius;
    Color color;
} Particle;

Vector2 gravity_force(Particle p_receive, Particle p_cause)
{
    Vector2 rel_pos = Vector2Subtract(p_cause.pos, p_receive.pos);
    Vector2 force_direction = Vector2Normalize(rel_pos);
    float dist = Vector2Length(rel_pos);
    float force_magnitude = G*p_receive.mass*p_cause.mass / (dist*dist);
    return Vector2Scale(force_direction, force_magnitude);
}

void DrawParticle(Particle p)
{
    float factor = 350.0f;
    Vector2 screen_pos = Vector2Scale(p.pos, factor);
    screen_pos = Vector2Add(screen_pos, (Vector2) { SCREEN_WIDTH/2, SCREEN_HEIGHT/2 });
    DrawCircleV(screen_pos, p.radius, p.color); 
}

void update_particle(Particle *sys, int i, float dt)
{
    Vector2 res_force = Vector2Zero();
    for (int j = 0; j < 2; j++) {
        if (j != i) {
            Vector2 force = gravity_force(sys[i], sys[j]);
            res_force = Vector2Add(res_force, force);
        }
    }
    sys[i].vel = Vector2Add(sys[i].vel, Vector2Scale(res_force, dt/sys[i].mass));
    sys[i].pos = Vector2Add(sys[i].pos, Vector2Scale(sys[i].vel, dt));    
}

float dist(Particle *sys, int i)
{
    float ke = sys[i].mass * Vector2LengthSqr(sys[i].vel) / 2.0f;
    float pe_sun = - G * M_SUN * sys[i].mass / Vector2Length(sys[i].pos);
    Vector2 dist_jup = Vector2Subtract(sys[i].pos, sys[1].pos);
    float pe_jup = - G * sys[1].mass * sys[i].mass / Vector2Length(dist_jup);
    return -2 * G * M_SUN * sys[2].mass / (ke + pe_sun + pe_jup);
}

void initialize_sys(Particle *sys)
{
    Particle sun = {
        .pos = {0, 0},
        .vel = {0},
        .mass = M_SUN,
        .radius = 20,
        .color = YELLOW    
    };
    Particle jupiter = {
        .pos = {1.0f, 0},
        .vel = {0, 1.0f},
        .mass = 9.54e-4 * M_SUN,
        .radius = 5,
        .color = BROWN
    };
    sys[0] = sun;
    sys[1] = jupiter;

    const float asteroid_mass = 1e-10;
    const float rmin = 2.0f/5.2f;
    const float rmax = 3.5f/5.2f;
    const float dr = (rmax - rmin)/NUM_ASTEROIDS;
    for (int i = 0; i < NUM_ASTEROIDS; i++) {
        float angle = rand() / (float) RAND_MAX * 360.0f;
        float r = rmin + i*dr;
        float vel = sqrtf(G * M_SUN / r);
        Particle asteroid = {
            .pos = Vector2Rotate((Vector2){r, 0}, angle),
            .vel = Vector2Scale(Vector2Rotate((Vector2){0, vel}, angle), 1.05),
            .mass = asteroid_mass,
            .radius = 1,
            .color = GRAY
        };
        sys[2+i] = asteroid;
    }
}

void update_sys(Particle *sys, int steps, float dt)
{
    for (int j = 0; j < steps; j++) {
        #pragma omp parallel for
        for (int i = 0; i < NUM_ASTEROIDS + 2; i++) {
            if (i > 0) update_particle(sys, i, dt);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("USAGE: ./kirkwood <MODE>\n");
        printf("    modes:\n");
        printf("        v: real-time visualization\n");
        printf("        p <N>: plot distance distribution after N steps. Note that one period of Jupiter is approximately 630 time steps.\n");
        printf("ERROR: could not find run mode.\n");
        exit(1);
    }

    Particle sys[NUM_ASTEROIDS + 2];
    initialize_sys(sys);

    if (*argv[1] == 'v') {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Kirkwood Gaps");
        SetTargetFPS(60);
        while (!WindowShouldClose())
        {
            float dt = GetFrameTime();
            update_sys(sys, 1, dt);

            // Render system
            BeginDrawing();
                ClearBackground(BLACK);
                for (int i = 0; i < NUM_ASTEROIDS + 2; i++) {
                    DrawParticle(sys[i]);
                }
            EndDrawing();
        }
        CloseWindow();
    }
    else if (*argv[1] == 'p')
    {
        if (argc < 3) {
            printf("ERROR: no number of steps provided\n");
            exit(1);
        }
        const int steps = atoi(argv[2]);
        const float dt = 0.01;
        printf("Evolving system for %d steps using dt = %.3f...\n", steps, dt);
        update_sys(sys, steps, dt);
        
        printf("Finished calculations, saving distance data to data.txt...\n");

        FILE *f = fopen("data.txt", "w");
        if (f == NULL) {
            printf("ERROR: could not open file data.txt");
            exit(1);
        }
        for (int i = 0; i < NUM_ASTEROIDS; i++) {
            float d = dist(sys, i+2);
            fprintf(f, "%f ", d);
        }
        fclose(f);
        printf("Saved distance data to data.txt...\n");
        printf("Calling python plot backend...\n");
        system("python3 plot.py");
        system("rm ./data.txt");
        printf("Simulation finished successfully!\n");
    }
    return 0;
}