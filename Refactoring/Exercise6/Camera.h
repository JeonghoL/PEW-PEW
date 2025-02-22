#pragma once

class Camera {
public:
    Camera();

    void handleAltKey(bool pressed);
    void handleMouseMovement(double cur_x, double cur_y);
    void handleScroll(double yoffset);
    void starting();

    glm::mat4 get3rdPersonViewMatrix(const glm::vec3& targetPos);
    glm::mat4 get1stPersonViewMatrix(const glm::vec3& targetPos);
    glm::vec3 getMouseWorldDirection(float cur_x, float cur_y,
        const glm::mat4& projection,
        const glm::mat4& view,
        const glm::vec3& targetPos);

    bool isAltPressed() const { return LeftAlt_on; }
    float getZoomLevel() const { return Rm; }
    float get_horizontal_angle() const { return camera_horizontal_angle; }
    float get_vertical_angle() const { return camera_vertical_angle; }
    float get_start_pos() const { return start_pos; }
    glm::vec3 getMousePicking(float mouseX, float mouseY, const glm::mat4& projection, const glm::mat4& view);
    
    void addfinishpos();
    void setInitialDirection(const glm::vec3& direction);

    glm::vec3 getPosition(const glm::vec3& targetPos);
private:
    float Rm;
    bool LeftAlt_on;
    bool first_click;
    double first_cur_x, first_cur_y;
    float camera_horizontal_angle;
    float camera_vertical_angle;
    float start_pos{ 30.0f };
    float finish_pos{ 0.0f };

    const float WIN_W = 2560.0f;
    const float WIN_H = 1440.0f;
    const float PI = 3.141592f;
};