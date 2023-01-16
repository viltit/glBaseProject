/*	class to transform the game objects (rotate, scale, translate) */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace moe {

/*
 * TODO: Quaternion is overkill in 2D
*/
	class Transform {
	public:
		Transform()
			: _scale{ 1.0f, 1.0f, 1.0f },
			  _o{ 1, 0, 0, 0 }
		{ }

		Transform(const glm::vec3& pos, const glm::vec3 scale = glm::vec3{ 1.0f, 1.0f, 1.0f })
			: _pos{ pos },
			  _scale{ scale },
			  _o{ 1, 0, 0, 0 }
		{ }

		/* setters: */
		void	setPos(const glm::vec3& pos) 			{ _pos = pos; }
		void	move(const glm::vec3& distance) 		{ _pos += distance; }

		/** note that angle is in degrees */
		void	rotate(float angle, const glm::vec3& axis) {
			_o = glm::rotate(_o, glm::radians(angle), axis);
			_o = glm::normalize(_o);
		}

		void	rotate(const glm::quat& q) {
			_o = _o * q;
		}

		void	setRotation(const glm::quat& q) { _o = q; }
		// TODO: Look at
		// TODO: smootMove and smootRotate?

		void	setScale(const glm::vec3& scale) { _scale = scale; }

		/* getters: */
		glm::vec3 pos() const { return _pos; }
		glm::vec3 scale() const { return _scale; }

		const glm::mat4 worldMatrix() const {
			// TODO: Store W, only update when dirty
			// TODO: Make sure we have the right order
			glm::mat4 W{ 1 };
			W = glm::scale(W, _scale);
			W = glm::toMat4(_o) * W;
			W = glm::translate(W, _pos);		
			return W;
		}

		glm::quat orientation() const {
			return _o;
		}

	private:
		glm::vec3 _pos;
		glm::vec3 _scale;
		glm::quat _o;
	};
}