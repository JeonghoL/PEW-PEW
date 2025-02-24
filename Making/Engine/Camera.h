#pragma once

class Camera {
public:
    Camera();

    void Update();
    //void handleAltKey(bool pressed);
    //void handleMouseMovement(double cur_x, double cur_y);
    //void handleScroll(double yoffset);
    //void starting();

    //glm::mat4 get3rdPersonViewMatrix(const glm::vec3& targetPos);
    //glm::mat4 get1stPersonViewMatrix(const glm::vec3& targetPos);
    //glm::vec3 getMouseWorldDirection(float cur_x, float cur_y, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& targetPos);

    //bool isAltPressed() const { return LeftAlt_on; }
    //float getZoomLevel() const { return Rm; }
    //float get_horizontal_angle() const { return camera_horizontal_angle; }
    //float get_vertical_angle() const { return camera_vertical_angle; }
    //float get_start_pos() const { return start_pos; }
    //glm::vec3 getMousePicking(float mouseX, float mouseY, const glm::mat4& projection, const glm::mat4& view);

    //void addfinishpos();
    //void setInitialDirection(const glm::vec3& direction);
    void RotRight(bool in) { rotRight = in; };
	void RotLeft(bool in) { rotLeft = in; };
	void RotUp(bool in) { rotUp = in; };
	void RotDown(bool in) { rotDown = in; };

    glm::vec3 getPosition();
	glm::mat4 getViewMatrix();

private:
    float Rm = { 1.0f };
    bool LeftAlt_on = { false };
    bool first_click = { false };
    double first_cur_x = { 0.0f };
    double first_cur_y = { 0.0f };
    float camera_horizontal_angle = { 0.0f };
    float camera_vertical_angle = { 0.0f };
    float start_pos = { 30.0f };
    float finish_pos = { 0.0f };

    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    float yaw = { 0.0f };
	float pitch = { 0.0f };

	bool rotRight = { false };
	bool rotLeft = { false };
	bool rotUp = { false };
	bool rotDown = { false };
};