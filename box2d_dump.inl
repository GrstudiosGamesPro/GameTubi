b2Vec2 g(0, 20);
m_world->SetGravity(g);
b2Body** bodies = (b2Body**)b2Alloc(2 * sizeof(b2Body*));
b2Joint** joints = (b2Joint**)b2Alloc(0 * sizeof(b2Joint*));
{
  b2BodyDef bd;
  bd.type = b2BodyType(2);
  bd.position.Set(0, 0);
  bd.angle = 0;
  bd.linearVelocity.Set(0, 0);
  bd.angularVelocity = 0;
  bd.linearDamping = 0;
  bd.angularDamping = 0;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.enabled = bool(32);
  bd.gravityScale = 1;
  bodies[0] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 0.300000012;
    fd.restitution = 0;
    fd.restitutionThreshold = 1;
    fd.density = 1;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-2.5, -2.5);
    vs[1].Set(2.5, -2.5);
    vs[2].Set(2.5, 2.5);
    vs[3].Set(-2.5, 2.5);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
}
{
  b2BodyDef bd;
  bd.type = b2BodyType(2);
  bd.position.Set(6.48371315, 14.9301443);
  bd.angle = 0;
  bd.linearVelocity.Set(0, 0);
  bd.angularVelocity = 0;
  bd.linearDamping = 0;
  bd.angularDamping = 0;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.enabled = bool(32);
  bd.gravityScale = 1;
  bodies[1] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 0.300000012;
    fd.restitution = 0;
    fd.restitutionThreshold = 1;
    fd.density = 1;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-0.5, -0.5);
    vs[1].Set(0.5, -0.5);
    vs[2].Set(0.5, 0.5);
    vs[3].Set(-0.5, 0.5);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 0.300000012;
    fd.restitution = 0;
    fd.restitutionThreshold = 1;
    fd.density = 1;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-0.5, -0.5);
    vs[1].Set(0.5, -0.5);
    vs[2].Set(0.5, 0.5);
    vs[3].Set(-0.5, 0.5);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
}
b2Free(joints);
b2Free(bodies);
joints = nullptr;
bodies = nullptr;
