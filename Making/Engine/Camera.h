#pragma once

class Camera {
public:
    Camera();

    void Update();
    //void handleAltKey(bool pressed);
    void handleMouseMovement(double cur_x, double cur_y);
    //void handleScroll(double yoffset);
    void starting();

    glm::mat4 Get1stPersonViewMatrix(const glm::vec3& targetPos);
    glm::mat4 Get3rdPersonViewMatrix(const glm::vec3& targetPos);
    glm::vec3 GetMouseWorldDirection(float cur_x, float cur_y, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& targetPos);

    bool IsAltPressed() const { return LeftAlt_on; }
    //float getZoomLevel() const { return Rm; }
    float GetHorizontalAngle() const { return camera_horizontal_angle; }
    float GetVerticalAngle() const { return camera_vertical_angle; }
    //float get_start_pos() const { return start_pos; }
    //glm::vec3 getMousePicking(float mouseX, float mouseY, const glm::mat4& projection, const glm::mat4& view);

    //void addfinishpos();
    void SetInitialDirection(const glm::vec3& direction);
    
    void ChangeViewType() { FirstPersonView = !FirstPersonView; }
	glm::vec3 SetMouseWorldDirection(float cur_x, float cur_y, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& targetPos);

	bool GetViewType() { return FirstPersonView; }

    glm::vec3 getPosition(const glm::vec3& targetPos);
	glm::mat4 getViewMatrix(const glm::vec3& targetPos);

    void SetAngle();
	void SetStart(bool in) { start = in; }

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

    bool FirstPersonView = { false };

	float angle = { 0.0f };
    float light_angle = { 0.0f };

    bool start{ false };
};