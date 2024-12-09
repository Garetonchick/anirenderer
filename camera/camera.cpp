#include <camera/camera.h>

#include <algorithm>
#include <cmath>
#include <glm/trigonometric.hpp>
#include "glm/ext/matrix_transform.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/geometric.hpp"

namespace ani {

Camera::Camera(const glm::vec3& pos, float pitch, float yaw, float min_pitch, float max_pitch)
    : pos_(pos), pitch_(pitch), yaw_(yaw), min_pitch_(min_pitch), max_pitch_(max_pitch) {
    NormalizeAngles();
}

const glm::vec3& Camera::GetPos() const {
    return pos_;
}

float Camera::GetPitch() const {
    return pitch_;
}

float Camera::GetYaw() const {
    return yaw_;
}

const glm::mat4& Camera::GetViewMatrix() const {
    return view_;
}

const glm::mat4& Camera::GetProjMatrix() const {
    return proj_;
}

const glm::mat4& Camera::GetViewProjMatrix() const {
    return view_proj_;
}

glm::vec3 Camera::GetDirectionVec() const {
    float rpitch = glm::radians(pitch_);
    float ryaw = glm::radians(yaw_);

    return glm::normalize(
        glm::vec3{sin(ryaw) * cos(rpitch), sin(rpitch), -cos(ryaw) * cos(rpitch)});
}

glm::vec3 Camera::GetRightVec() const {
    return glm::normalize(glm::cross(GetDirectionVec(), kGlobalUp));
}

glm::vec3 Camera::GetUpVec() const {
    return glm::normalize(glm::cross(GetRightVec(), GetDirectionVec()));
}

void Camera::SetViewFrustrum(float yfov, float width_to_height_ratio, float near, float far) {
    proj_ = glm::perspective(glm::radians(yfov), width_to_height_ratio, near, far);
    RecalcMatricies();
}

void Camera::Move(const glm::vec3& pos_dlt) {
    pos_ += pos_dlt;
}

void Camera::Rotate(float pitch_dlt, float yaw_dlt) {
    pitch_ += pitch_dlt;
    yaw_ += yaw_dlt;
    NormalizeAngles();
    RecalcMatricies();
}

void Camera::NormalizeAngles() {
    pitch_ = std::clamp(pitch_, min_pitch_, max_pitch_);

    if (yaw_ <= 0.f) {
        yaw_ += 360.f;
    } else if (yaw_ >= 360.f) {
        yaw_ -= 360.f;
    }
}

void Camera::RecalcMatricies() {
    view_ = CalcViewMatrix();
    view_proj_ = proj_ * view_;
}

glm::mat4 Camera::CalcViewMatrix() {
    return glm::lookAt(pos_, pos_ + GetDirectionVec(), GetUpVec());
}

}  // namespace ani
