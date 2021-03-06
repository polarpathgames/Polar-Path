#include "App.h"
#include "m1Render.h"
#include "c1CutsceneMoveCamera.h"
#include "m1Cutscene.h"
#include "m1Scene.h"
#include "m1Map.h"
#include "m1Window.h"
#include "e1Player.h"
#include "p2Point.h"
#include "p2Math.h"

c1CutsceneMoveCamera::c1CutsceneMoveCamera(uint start, uint duration, int origin_x, int origin_y, int destination_x, int destination_y, float speed, bool stop_when_goal, bool from_player) :c1CutsceneAction(start, duration), 
	speed(speed), stop_when_goal(stop_when_goal)
{
	camera_destination.create(destination_x, destination_y);
	if (from_player) {
		iPoint screen = App->map->WorldToMap(App->scene->player->position.x, App->scene->player->position.y);
		camera_origin.create(screen.x, screen.y);
	}
		
	
	else
		camera_origin.create(origin_x, origin_y);
}

void c1CutsceneMoveCamera::Execute(float dt)
{
	double start = App->cutscene_manager->GetTimer() - start_time;

	if (start < duration_time)
	{
		lerp_by += speed;
		iPoint position = lerp(camera_origin, camera_destination, lerp_by).AproximateToIntCast();
		App->render->camera.x = position.x;
		App->render->camera.y = position.y;

		if (stop_when_goal) {
			if (lerp_by >= 1.0f) {
				duration_time = 0u;
			}
		}
	}
}
