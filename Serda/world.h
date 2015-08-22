#ifndef SERDA_WORLD_H
#define SERDA_WORLD_H

#include <vector>
#include <unordered_map>

#include "entity.h"

class World {
private:
  // Class to iterate over values of entities.
  typedef std::unordered_map<id_type, Entity> entityMap;

  class value_iterator : public entityMap::iterator {
  public:
    value_iterator() : entityMap::iterator() {}
    value_iterator(entityMap::iterator e) : entityMap::iterator(e) {};
    Entity* operator->() {
      return (Entity* const) & (entityMap::iterator::operator->()->second);
    }
    Entity operator*() { return entityMap::iterator::operator*().second; }
  };

public:
  // Range to allow for range loop in entities.
  struct Range {
    std::unordered_map<id_type, Entity>& entities;
    Range(std::unordered_map<id_type, Entity>& entities) : entities(entities) {}
    value_iterator begin() { return entities.begin(); }
    value_iterator end() { return entities.end(); }
  };

  static const id_type DYNAMIC_ID_RANGE_FROM;
  static const id_type DYNAMIC_ID_RANGE_TO;

  // Constructors and destructors.
  World();
  ~World();

  // Entity operations.
  void createEntity(id_type entity_id);
  id_type createEntity();

  const Entity& getEntity(id_type entity_id) const;
  Range getEntities();


  // Compenent queries.
  bool hasComponent(const Entity& entity, Component::Id cid) const;
  bool hasComponent(id_type entity_id, Component::Id cid) const;

  // TODO: implement these methods.
  bool isActive(const Entity& entity, Component::Id cid) const;
  bool isActive(id_type entity_id, Component::Id cid) const;

  void activateComponent(const Entity& entity, Component::Id cid) const;
  void activateComponent(id_type entity_id, Component::Id cid) const;

  void deactivateComponent(const Entity& entity, Component::Id cid) const;
  void deactivateComponent(id_type entity_id, Component::Id cid) const;

  // Component gettters.
  const Component& getComponent(const Entity& entity, Component::Id cid) const;
  const Component& getComponent(id_type entity_id, Component::Id cid) const;
  Component& mutableComponent(const Entity& entity, Component::Id cid);
  Component& mutableComponent(id_type entity_id, Component::Id cid);

  // PositionComponent getter specialization.
  const PositionComponent& getPositionComponent(const Entity& entity) const;
  const PositionComponent& getPositionComponent(id_type entity_id) const;
  PositionComponent& mutablePositionComponent(const Entity& entity);
  PositionComponent& mutablePositionComponent(id_type entity_id);

  // SpeedComponent getter specialization.
  const SpeedComponent& getSpeedComponent(const Entity& entity) const;
  const SpeedComponent& getSpeedComponent(id_type entity_id) const;
  SpeedComponent& mutableSpeedComponent(const Entity& entity);
  SpeedComponent& mutableSpeedComponent(id_type entity_id);

  // RenderComponent getter specialization.
  const RenderComponent& getRenderComponent(const Entity& entity) const;
  const RenderComponent& getRenderComponent(id_type entity_id) const;
  RenderComponent& mutableRenderComponent(const Entity& entity);
  RenderComponent& mutableRenderComponent(id_type entity_id);

  // InputComponent getter specialization.
  const InputComponent& getInputComponent(const Entity& entity) const;
  const InputComponent& getInputComponent(id_type entity_id) const;
  InputComponent& mutableInputComponent(const Entity& entity);
  InputComponent& mutableInputComponent(id_type entity_id);

  // All methods to add each component.
  void addComponent(id_type entity_id, PositionComponent pc);
  void addComponent(id_type entity_id, SpeedComponent sc);
  void addComponent(id_type entity_id, RenderComponent rc);
  void addComponent(id_type entity_id, InputComponent ic);

private:
  // Returns an random entityId not being used.
  id_type getRandomEntityId();

  // TODO: cache problems, maybe.
  std::unordered_map<id_type, Entity> entities;

  Range range;

  // Components go here.
  std::vector<PositionComponent> position_components;
  std::vector<SpeedComponent> speed_components;
  std::vector<RenderComponent> render_components;
  std::vector<InputComponent> input_components;
};

#endif  // SERDA_WORLD_H
