#include "GameObject3D.h"
#include "World3D.h"

// Implementation of Player3D::Update method
void Player3D::Update(double deltaTime, World3D* world) {
    if (!active) return;

    // Apply gravity
    if (world) {
        Vector3D gravity = world->getGravity();
        velocity += gravity * deltaTime;
    }

    // Handle movement input
    if (inputDirection.magnitude() > 0.1) {
        // Calculate movement direction relative to player's facing direction
        Vector3D forward = transform.getForward();
        Vector3D right = transform.getRight();
        
        // Remove Y component for ground movement
        forward.y = 0;
        right.y = 0;
        forward = forward.normalize();
        right = right.normalize();
        
        // Calculate movement vector
        Vector3D moveDirection = (forward * inputDirection.z + right * inputDirection.x).normalize();
        
        // Apply movement
        Vector3D moveVelocity = moveDirection * moveSpeed;
        velocity.x = moveVelocity.x;
        velocity.z = moveVelocity.z;
        
        // Rotate player to face movement direction if moving
        if (moveDirection.magnitude() > 0.1) {
            double targetYaw = std::atan2(moveDirection.x, moveDirection.z) * 180.0 / M_PI;
            
            // Smooth rotation towards target
            double currentYaw = transform.rotation.y;
            double yawDiff = targetYaw - currentYaw;
            
            // Handle angle wrapping
            while (yawDiff > 180.0) yawDiff -= 360.0;
            while (yawDiff < -180.0) yawDiff += 360.0;
            
            // Apply rotation
            double rotationAmount = rotationSpeed * deltaTime;
            if (std::abs(yawDiff) < rotationAmount) {
                transform.rotation.y = targetYaw;
            } else {
                transform.rotation.y += (yawDiff > 0) ? rotationAmount : -rotationAmount;
            }
            
            // Keep rotation in 0-360 range
            while (transform.rotation.y >= 360.0) transform.rotation.y -= 360.0;
            while (transform.rotation.y < 0.0) transform.rotation.y += 360.0;
        }
    } else {
        // No input - stop horizontal movement (with some friction)
        velocity.x *= 0.8;
        velocity.z *= 0.8;
    }

    // Store old position for collision resolution
    Vector3D oldPosition = transform.position;
    
    // Apply velocity to position
    transform.position += velocity * deltaTime;
    
    // World bounds checking
    if (world) {
        if (!world->isInBounds(transform.position)) {
            transform.position = world->clampToBounds(transform.position);
            
            // Stop velocity if hitting world bounds
            if (transform.position.x <= world->getBounds().min.x || 
                transform.position.x >= world->getBounds().max.x) {
                velocity.x = 0;
            }
            if (transform.position.z <= world->getBounds().min.z || 
                transform.position.z >= world->getBounds().max.z) {
                velocity.z = 0;
            }
            if (transform.position.y <= world->getBounds().min.y) {
                transform.position.y = world->getBounds().min.y;
                velocity.y = 0;
                onGround = true;
            }
        }
        
        // Platform collision detection
        Vector3D correctionOffset;
        if (world->checkPlatformCollision(transform.position, size, correctionOffset)) {
            transform.position += correctionOffset;
            
            // Stop velocity in the direction of collision
            if (std::abs(correctionOffset.y) > std::abs(correctionOffset.x) && 
                std::abs(correctionOffset.y) > std::abs(correctionOffset.z)) {
                // Vertical collision
                if (correctionOffset.y > 0) {
                    // Landing on top of platform
                    velocity.y = 0;
                    onGround = true;
                } else {
                    // Hitting platform from below
                    velocity.y = 0;
                }
            } else if (std::abs(correctionOffset.x) > std::abs(correctionOffset.z)) {
                // Horizontal collision (X-axis)
                velocity.x = 0;
            } else {
                // Horizontal collision (Z-axis)
                velocity.z = 0;
            }
        } else {
            // Check if still on ground
            onGround = world->isOnGround(transform.position, size);
        }
    }
    
    // Reset input direction for next frame
    inputDirection = Vector3D::Zero();
}
