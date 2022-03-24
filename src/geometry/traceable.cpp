#include "traceable.h"

#include <algorithm>

using std::list;
using std::sort;
using std::vector;

/*****************************
 * 		 Transformer
 * ***************************/

Vec3f Transformer::normal(const Point3f &p) const {
  Vec4f v4(p);
  v4 = m_transformer.inverse() * v4;
  Point3f object_point = v4;
  Vec3f object_normal = object_point - Point3f(0.0f, 0.0f, 0.0f);
  v4 = object_normal;
  v4 = m_transformer.inverse().transpose() * v4;
  Vec3f world_normal = v4;
  return world_normal.normalize();
  // return TraceableDeco::normal(p);
}

/*****************************
 * 		    Material
 * ***************************/

Material::Material(Traceable *tr, const Vec3f &c, float am, float diff,
                   float spec, float shi)
    : TraceableDeco(tr),
      m_color(c),
      m_ambient(am),
      m_diffuse(diff),
      m_specular(spec),
      m_shininess(shi) {}

Vec3f Material::lighting(const PointLight &light, const Ray &ray) {
  Vec3f effective_color = m_color * light.intensity();
  Point3f p = record().point(ray);
  Vec3f lightv = (light.position() - p).normalize();

  Vec3f ret_ambient = effective_color * m_ambient;
  Vec3f ret_diffuse;
  Vec3f ret_specular;
  Vec3f normal_vec = record().inside ? -normal(p) : normal(p);

  float light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse = effective_color * m_diffuse * light_normal;
    Vec3f reflectv = reflect(-lightv, normal_vec);
    float reflect_dot_eye = dot(reflectv, record().eye(ray));
    if (reflect_dot_eye > 0.0f) {
      float factor = pow(reflect_dot_eye, m_shininess);
      ret_specular = light.intensity() * m_specular * factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

/*****************************
 * 		    World
 * ***************************/

World::~World() {
  list<Traceable *>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    delete (*it);
}

bool World::intersect(const Ray &r) {
  bool has_intersection = false;
  list<Traceable *>::iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
    if ((*it)->intersect(r)) has_intersection = true;
  return has_intersection;
}

void World::add(Traceable *item) {
  m_traceable_list.push_back(item);
  item->setParent(this);
}

void World::remove(Traceable *item, bool del) {
  m_traceable_list.remove(item);
  item->setParent(nullptr);
  if (del) delete item;
}

Traceable &World::closestHit() {
  list<Traceable *>::const_iterator it;
  Traceable *temp = nullptr;
  float min_hit = MAXFLOAT;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    if ((*it)->record().t_min() > 0.0f && (*it)->record().t_min() < min_hit) {
      temp = (*it);
      min_hit = temp->record().t_min();
    }
  }
  if (temp == nullptr) {
    temp = m_traceable_list.front();  // just return the first one.
  }
  return *temp;
}

bool compare(const float &f1, const float &f2) { return f1 < f2; }

vector<float> World::intersectionsSorted() const {
  vector<float> ret;
  list<Traceable *>::const_iterator it;
  for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it) {
    ret.push_back((*it)->record().t1);
    ret.push_back((*it)->record().t2);
  }
  sort(ret.begin(), ret.end(), compare);
  return ret;
}
