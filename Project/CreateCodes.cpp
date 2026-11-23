#include"CreateCodes.h"
Robot* createRobot() {
    Mesh* box = pg.loadMeshlFromObj("assets/models/primitives/box.obj");

    if (!box) {
        std::cerr << "box.obj load failed\n";
        return nullptr;
    }

    // 원본 상자 데이터를 보관
    const auto vertices = box->vertices;
    const auto indices = box->indices;
    const auto textures = box->textures;

    struct Part {
        std::string name;
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 offset;
        glm::vec3 color;
    };

    Part descriptions[] = {
        { "torso",
          {0.f, 2.5f, 0.f}, {1.25f, 1.4f, 0.7f},
          {0.f, 0.f, 0.f}, {0.08f, 0.48f, 0.68f} },

        { "head",
          {0.f, 1.15f, 0.f}, {0.9f, 0.9f, 0.8f},
          {0.f, 0.f, 0.f}, {0.83f, 0.88f, 0.91f} },

        { "leftShoulder",
          {-0.88f, 0.6f, 0.f}, {0.4f, 1.3f, 0.48f},
          {0.f, -0.65f, 0.f}, {0.95f, 0.48f, 0.13f} },

        { "rightShoulder",
          {0.88f, 0.6f, 0.f}, {0.4f, 1.3f, 0.48f},
          {0.f, -0.65f, 0.f}, {0.96f, 0.79f, 0.18f} },

        { "leftHip",
          {-0.38f, -0.7f, 0.f}, {0.5f, 1.8f, 0.55f},
          {0.f, -0.9f, 0.f}, {0.52f, 0.27f, 0.72f} },

        { "rightHip",
          {0.38f, -0.7f, 0.f}, {0.5f, 1.8f, 0.55f},
          {0.f, -0.9f, 0.f}, {0.25f, 0.62f, 0.28f} },

        { "nose",
        {0.f, 0.f, 0.5f}, {0.22f, 0.22f, 0.24f},
        {0.f, 0.f, 0.f}, {0.95f, 0.48f, 0.13f}
},
    };

    Robot* robot = new Robot();

    for (Part& part : descriptions) {
        Mesh* mesh = robot->meshes.empty() ? box : new Mesh(vertices, indices, textures);

        for (Vertex& vertex : mesh->vertices) {
            vertex.position = vertex.position * part.size + part.offset;
        }

        mesh->color = part.color;
        mesh->transform =
            glm::translate(glm::mat4(1.f), part.position);
        mesh->updateBuffer();

        Node* node = new Node(mesh, part.position);
        node->rotation = glm::vec3(0.f);

        if (!robot->root) {
            robot->root = node;
        }
        else if (part.name == "nose") {
            robot->getJoint("head")->child.push_back(node);
        }
        else {
            robot->root->child.push_back(node);
        }

        robot->meshes.push_back(mesh);
        robot->resigt(part.name, node);
    }
    models.push_back(robot);
    return robot;
}

Mesh* createRoom(float size, float height) {
    Mesh* room = pg.loadMeshlFromObj("assets/models/environment/openTopRoom.obj");
    if (!room) return nullptr;

    room->color = glm::vec3(0.5f);
    room->textures.push_back(textureLoader.loadImg("assets/textures/environment/roomColors.png"));
    room->transform =
        glm::translate(glm::mat4(1.f), glm::vec3(0.f, 3.f, 0.f))
        * glm::scale(glm::mat4(1.f), glm::vec3(size, height, size));
    meshes.push_back(room);
    return room;
}

Truck* createTruck() {
    Mesh* source = pg.loadMeshlFromObj("assets/models/primitives/box.obj");
    if (!source) {
        std::cerr << "box.obj load failed for truck\n";
        return nullptr;
    }

    struct TruckPart {
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 color;
    };
    const TruckPart parts[] = {
        {{0.0f, 0.86f, -0.05f}, {2.1f, 0.42f, 6.1f}, {0.12f, 0.14f, 0.16f}}, // chassis
        {{0.0f, 2.0f, 2.2f}, {2.2f, 2.0f, 1.6f}, {0.1f, 0.4f, 0.68f}}, // cab
        {{0.0f, 3.07f, 2.2f}, {2.32f, 0.14f, 1.72f}, {0.08f, 0.3f, 0.52f}}, // roof
        {{0.0f, 2.47f, 3.025f}, {1.82f, 0.66f, 0.05f}, {0.3f, 0.7f, 0.87f}}, // frontWindow
        {{-1.125f, 2.47f, 2.2f}, {0.05f, 0.66f, 1.1f}, {0.3f, 0.7f, 0.87f}}, // leftWindow
        {{1.125f, 2.47f, 2.2f}, {0.05f, 0.66f, 1.1f}, {0.3f, 0.7f, 0.87f}}, // rightWindow
        {{0.0f, 1.05f, 3.12f}, {2.44f, 0.22f, 0.22f}, {0.38f, 0.42f, 0.46f}}, // frontBumper
        {{0.0f, 1.56f, 3.025f}, {1.12f, 0.32f, 0.05f}, {0.1f, 0.12f, 0.14f}}, // grille
        {{-0.78f, 1.55f, 3.04f}, {0.34f, 0.24f, 0.08f}, {1.0f, 0.91f, 0.6f}}, // leftHeadlight
        {{0.78f, 1.55f, 3.04f}, {0.34f, 0.24f, 0.08f}, {1.0f, 0.91f, 0.6f}}, // rightHeadlight
        {{0.0f, 1.36f, -0.9f}, {2.4f, 0.2f, 4.4f}, {0.44f, 0.48f, 0.51f}}, // cargoFloor
        {{-1.14f, 1.81f, -0.9f}, {0.12f, 0.7f, 4.4f}, {0.25f, 0.42f, 0.49f}}, // cargoLeft
        {{1.14f, 1.81f, -0.9f}, {0.12f, 0.7f, 4.4f}, {0.25f, 0.42f, 0.49f}}, // cargoRight
        {{0.0f, 1.81f, 1.24f}, {2.16f, 0.7f, 0.12f}, {0.25f, 0.42f, 0.49f}}, // cargoFront
        {{0.0f, 1.81f, -3.04f}, {2.16f, 0.7f, 0.12f}, {0.25f, 0.42f, 0.49f}}, // cargoRear
        {{-0.8f, 1.2f, -3.12f}, {0.3f, 0.18f, 0.05f}, {0.78f, 0.08f, 0.06f}}, // leftTailLight
        {{0.8f, 1.2f, -3.12f}, {0.3f, 0.18f, 0.05f}, {0.78f, 0.08f, 0.06f}}, // rightTailLight
        {{-1.24f, 0.55f, 2.1f}, {0.38f, 1.1f, 1.1f}, {0.055f, 0.06f, 0.065f}}, // wheel
        {{-1.445f, 0.55f, 2.1f}, {0.04f, 0.48f, 0.48f}, {0.48f, 0.52f, 0.55f}}, // hub
        {{1.24f, 0.55f, 2.1f}, {0.38f, 1.1f, 1.1f}, {0.055f, 0.06f, 0.065f}}, // wheel
        {{1.445f, 0.55f, 2.1f}, {0.04f, 0.48f, 0.48f}, {0.48f, 0.52f, 0.55f}}, // hub
        {{-1.24f, 0.55f, -2.1f}, {0.38f, 1.1f, 1.1f}, {0.055f, 0.06f, 0.065f}}, // wheel
        {{-1.445f, 0.55f, -2.1f}, {0.04f, 0.48f, 0.48f}, {0.48f, 0.52f, 0.55f}}, // hub
        {{1.24f, 0.55f, -2.1f}, {0.38f, 1.1f, 1.1f}, {0.055f, 0.06f, 0.065f}}, // wheel
        {{1.445f, 0.55f, -2.1f}, {0.04f, 0.48f, 0.48f}, {0.48f, 0.52f, 0.55f}}, // hub
    };

    Truck* truck = new Truck();
    for (const TruckPart& part : parts) {
        Mesh* mesh = truck->meshes.empty()
            ? source
            : new Mesh(source->vertices, source->indices, std::vector<Texture>{});
        mesh->textures.clear();
        mesh->color = part.color;
        mesh->transform =
            glm::translate(glm::mat4(1.f), part.position)
            * glm::scale(glm::mat4(1.f), part.size);
        truck->meshes.push_back(mesh);
    }

    // Same registration convention as createRobot().
    models.push_back(truck);
    return truck;
}

Conveyor* loadConveyor() {
    Mesh* mesh = pg.loadMeshlFromObj("assets/models/conveyors/straight.obj");
    if (!mesh) return nullptr;

    Conveyor* con = new Conveyor();
    mesh->textures.push_back(texture);
    con->meshes.push_back(mesh);
    models.push_back(con);
    con->setStartAndEnd(mesh);

    Mesh* slatSource = pg.loadMeshlFromObj("assets/models/conveyors/slat.obj");
    if (!slatSource) return con;

    slatSource->textures.push_back(texture);

    float length = glm::length(con->end - con->start);
    int count = glm::max(1, int(length / con->spacing));

    for (int i = 0; i < count; ++i) {
        Mesh* slat = new Mesh(
            slatSource->vertices,
            slatSource->indices,
            slatSource->textures);

        float t = float(i) / float(count);
        glm::vec3 position = glm::mix(con->start, con->end, t);

        slat->transform = glm::translate(
            glm::mat4(1.f), position);

        con->slats.push_back(slat);  // 이동시킬 목록
        con->meshes.push_back(slat); // 실제 그릴 목록
    }
    return con;
}

Conveyor* loadCornerConveyor() {
    Mesh* mesh = pg.loadMeshlFromObj("assets/models/conveyors/corner.obj");
    if (!mesh) return nullptr;

    CornerConveyor* con = new CornerConveyor();
    mesh->textures.push_back(texture);
    con->meshes.push_back(mesh);
    models.push_back(con);

    Mesh* slatSource = pg.loadMeshlFromObj("assets/models/conveyors/slat.obj");
    if (!slatSource) return con;

    slatSource->textures.push_back(texture);

    float length = con->radius * glm::radians(90.f);
    int count = glm::max(1, int(length / con->spacing));
    con->conveySpeed = length / (count * con->spacing);
    for (int i = 0; i < count; ++i) {
        Mesh* slat = new Mesh(
            slatSource->vertices,
            slatSource->indices,
            slatSource->textures);

        con->slats.push_back(slat);  // 이동시킬 목록
        con->meshes.push_back(slat); // 실제 그릴 목록
    }
    con->update(0.f);
    return con;
}
BoxModel* createBox() {
    Mesh* box = pg.loadMeshlFromObj("assets/models/primitives/box.obj");
    if (!box) return nullptr;

    box->color = Random::randomColor();
    box->transform = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.5f, 0));
    BoxModel* bm = new BoxModel();
    bm->meshes.push_back(box);
    models.push_back(bm);
    return bm;
}

Mesh* createSphere() {
    Mesh* sphere = pg.loadMeshlFromObj("assets/models/primitives/sphere.obj");
    if (!sphere) return nullptr;

    meshes.push_back(sphere);
    return sphere;
}
