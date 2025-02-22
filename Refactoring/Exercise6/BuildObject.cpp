#include "BuildObject.h"

void BuildObject::generateFloor()
{
	for (float i = 0; i < 10; ++i)
	{
		for (float j = 0; j < 10; ++j)
		{
			Floor create_floor;
			create_floor.position = { -5.0f + (j * 1.0f),  0.0f, -5.0f + (i * 1.0f) };
			create_floor.normal = { 0.0f, 1.0f, 0.0f };
			create_floor.texCoord = { 0.0f, 0.0f };
			floorwall.push_back(create_floor);

			create_floor.position = { -5.0f + (j * 1.0f),  0.0f, -4.0f + (i * 1.0f) };
			create_floor.normal = { 0.0f, 1.0f, 0.0f };
			create_floor.texCoord = { 0.0f, 1.0f };
			floorwall.push_back(create_floor);

			create_floor.position = { -4.0f + (j * 1.0f),  0.0f, -4.0f + (i * 1.0f) };
			create_floor.normal = { 0.0f, 1.0f, 0.0f };
			create_floor.texCoord = { 1.0f, 1.0f };
			floorwall.push_back(create_floor);

			create_floor.position = { -4.0f + (j * 1.0f),  0.0f, -5.0f + (i * 1.0f) };
			create_floor.normal = { 0.0f, 1.0f, 0.0f };
			create_floor.texCoord = { 1.0f, 0.0f };
			floorwall.push_back(create_floor);
		}
	}
}