#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace ani {

class Camera {
public:
    Camera(
        const glm::vec3& pos = {}, 
        float pitch = 0.0f, 
        float yaw = 0.0f, 
        float min_pitch = kDefaultMinPitch, 
        float max_pitch = kDefaultMaxPitch
    );

    const glm::vec3& GetPos() const;
    float GetPitch() const;
    float GetYaw() const;
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjMatrix() const;
    const glm::mat4& GetViewProjMatrix() const;
    glm::vec3 GetDirectionVec() const;
    glm::vec3 GetRightVec() const;
    glm::vec3 GetUpVec() const;

    void SetViewFrustrum(float yfov, float width_to_height_ratio, float near, float far);

    void Move(const glm::vec3& pos_dlt);
    void Rotate(float pitch_dlt, float yaw_dlt);

private:
    void NormalizeAngles();
    void RecalcMatricies();
    glm::mat4 CalcProjMatrix();
    glm::mat4 CalcViewMatrix();

private:
    static constexpr glm::vec3 kGlobalUp = {0.f, 1.f, 0.f};
    static constexpr float kDefaultMinPitch = -89.f; 
    static constexpr float kDefaultMaxPitch = 89.f; 

    glm::vec3 pos_;
    float pitch_;
    float yaw_;

    const float min_pitch_;
    const float max_pitch_;

    glm::mat4 proj_;
    glm::mat4 view_;
    glm::mat4 view_proj_;
};

}  // namespace ani
