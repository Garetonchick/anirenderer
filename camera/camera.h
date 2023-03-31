#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace ani {

class Camera {
public:
    Camera(const glm::vec3& pos = {}, float pitch = 0.0f, float yaw = 0.0f, float min_pitch = -89.f,
           float max_pitch = 89.f);

    const glm::vec3& GetPos() const;
    float GetPitch() const;
    float GetYaw() const;
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetDirectionVec() const;
    glm::vec3 GetRightVec() const;
    glm::vec3 GetUpVec() const;

    void Move(const glm::vec3& pos_dlt);
    void Rotate(float pitch_dlt, float yaw_dlt);

private:
    void UpdateAngles();

private:
    static constexpr glm::vec3 kGlobalUp = {0.f, 1.f, 0.f};

    glm::vec3 pos_;
    float pitch_;
    float yaw_;

    const float min_pitch_;
    const float max_pitch_;
};

}  // namespace ani