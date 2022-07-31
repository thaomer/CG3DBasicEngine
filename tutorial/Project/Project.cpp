#include "Project.h"
#include <iostream>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Project::Project()
{
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Project::Init()
{		
	unsigned int texIDs[4] = { 0 , 1, 2 ,3};
	unsigned int slots[4] = { 0 , 1, 2 ,3};

	AddShader("shaders/pickingShader");
	AddShader("shaders/basicShader");
	AddShader("shaders/cubemapShader");
	
	AddTexture("textures/plane.png",2);
	AddTexture("textures/grass.bmp", 2);
	AddTexture("textures/cubemaps/Yokohama2_", 3);
	AddTexture("textures/cubemaps/Daylight Box_", 3);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	AddMaterial(texIDs + 3, slots + 3, 1);

	//lebackground
	AddShape(Cube, -2, TRIANGLES);
	SetShapeMaterial(0, 2);
	SetShapeShader(0, 2);
	selected_data_index = 0;
	ShapeTransformation(scaleAll, 60, 0);
	SetShapeStatic(0);


	AddShape(Cube,-1 , TRIANGLES);
	SetShapeMaterial(1, 1);
	SetShapeShader(1, 1);
	selected_data_index = 1;
	ShapeTransformation(xTranslate, -1, 0);
	ShapeTransformation(yTranslate, -1, 0);
	ShapeTransformation(zTranslate, -1, 0);

	AddShape(Cube, -1, TRIANGLES);
	SetShapeMaterial(2, 0);
	SetShapeShader(2, 1);

	selected_data_index = 2;
	ShapeTransformation(xTranslate, 1, 0);
	ShapeTransformation(yTranslate, 1, 0);
	ShapeTransformation(zTranslate, 1, 0);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


		s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);

	s->SetUniform1f("alpha", data_list[shapeIndx]->alpha);
	//textures[0]->Bind(0);

	
	

	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

void Project::Animate() {
    if(isActive)
	{
		if(selected_data_index > 0 )
			data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
	}
}

void Project::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}


Project::~Project(void)
{
}

