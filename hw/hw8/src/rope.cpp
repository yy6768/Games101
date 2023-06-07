#include "rope.h"

#include <iostream>
#include <vector>

#include "CGL/vector2D.h"
#include "mass.h"
#include "spring.h"

namespace CGL {

Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass,
           float k, vector<int> pinned_nodes) {
  // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and
  // containing `num_nodes` nodes.
  Vector2D delta = (end - start) / (num_nodes - 1);
  for (int i = 0; i < num_nodes; i++) {
    auto mass = new Mass(start + delta * i, node_mass, false);
    mass->velocity = Vector2D(0, 0);
    masses.emplace_back(mass);

    if (i != 0) {
      springs.emplace_back(new Spring(masses[i - 1], masses[i], k));
    }
  }

  // Comment-in this part when you implement the constructor
  for (auto &i : pinned_nodes) {
    masses[i]->pinned = true;
  }
}

void Rope::simulateEuler(float delta_t, Vector2D gravity) {
  for (auto &s : springs) {
    // TODO (Part 2): Use Hooke's law to calculate the force on a node
    Vector2D delta = s->m2->position - s->m1->position;
    Vector2D delta_force =
        s->k * delta / delta.norm() * (delta.norm() - s->rest_length);
    s->m1->forces += delta_force;
    s->m2->forces += -delta_force;
  }
  bool is_explicit = false;
  for (auto &m : masses) {
    if (!m->pinned) {
      // TODO (Part 2): Add the force due to gravity, then compute the new
      // velocity and position
      m->forces += gravity * m->mass;

      if (is_explicit) {
        float k_d = 7;  // damping
        m->forces += -k_d * m->velocity;
      } else {
        float k_d = 0.01;  // damping
        m->forces += -k_d * m->velocity;
      }
      Vector2D a = m->forces / m->mass;
      Vector2D v = m->velocity;
      if (is_explicit) {
        m->velocity += a * delta_t;
        m->position += v * delta_t;
      } else {
        m->velocity += a * delta_t;
        m->position += m->velocity * delta_t;
      }
      // TODO (Part 2): Add global damping
    }

    // Reset all forces on each mass
    m->forces = Vector2D(0, 0);
  }
}

void Rope::simulateVerlet(float delta_t, Vector2D gravity) {
  for (auto &s : springs) {
    // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet
    // （solving constraints)
    if (s->k != -1) {
      Vector2D delta = s->m2->position - s->m1->position;
      Vector2D delta_force =
          s->k * delta / delta.norm() * (delta.norm() - s->rest_length);
      s->m1->forces += delta_force;
      s->m2->forces += -delta_force;
    }
  }

  for (auto &m : masses) {
    if (!m->pinned) {
      Vector2D temp_position = m->position;
      // TODO (Part 3.1): Set the new position of the rope mass
      m->forces += gravity * m->mass;
      Vector2D a = m->forces / m->mass;
      Vector2D v = m->velocity;
      Vector2D x_0 = m->last_position;
      Vector2D x_1 = m->position;
      // Vector2D x_2 = x_1 + (x_1 - x_0) + a * delta_t * delta_t;
      // m->last_position = x_1;
      // m->position = x_2;
      // TODO (Part 4): Add global Verlet damping
      double damping = 0.00005;
      Vector2D x_2 = x_1 + (1 - damping) * (x_1 - x_0) + a * delta_t * delta_t;
      m->last_position = x_1;
      m->position = x_2;
    }
    m->forces = Vector2D(0, 0);
  }

  for (auto &s : springs) {
    if (s->k == -1) {
      Vector2D delta = s->m2->position - s->m1->position;
      Vector2D df_1 =
          0.5 * delta / delta.norm() * (delta.norm() - s->rest_length);
      Vector2D df_2 = -df_1;
      if (s->m1->pinned && s->m2->pinned) continue;
      if (s->m1->pinned) {
        df_2 *= 2;
        df_1 = Vector2D(0, 0);
      }
      if (s->m2->pinned) {
        df_1 *= 2;
        df_2 = Vector2D(0, 0);
      }

      s->m1->position += df_1;
      s->m2->position += df_2;
    }
  }
}
}  // namespace CGL
