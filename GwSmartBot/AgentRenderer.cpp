#include "AgentRenderer.h"

#include <SDL2\SDL_opengl.h>

void AgentRenderer::RenderAgents(const std::vector<AgentPosition>& agents) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	for (size_t i = 0; i < agents.size(); ++i) {
		AgentPosition agent = agents[i];
		float size = 50;
		glVertex2f(agent.x - size, agent.y + size);
		glVertex2f(agent.x + size, agent.y + size);
		glVertex2f(agent.x + size, agent.y - size);
		glVertex2f(agent.x - size, agent.y - size);
	}
	glEnd();
}

void AgentRenderer::RenderPlayer(const PseudoAgent& player) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	float size = 100;
	glVertex2f(player.X - size, player.Y + size);
	glVertex2f(player.X + size, player.Y + size);
	glVertex2f(player.X + size, player.Y - size);
	glVertex2f(player.X - size, player.Y - size);
	glEnd();
}
