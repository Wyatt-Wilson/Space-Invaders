#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* firstFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char* secondFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

double xNormalized = -5;
double yNormalized = -5;
bool gameStarted = false;
bool playerFound = false;
bool tntFound = false;
bool grayScale = false; //gray scale process include for juola
float diamondXLeft = -0.675;
float diamondXRight = -0.595;
float diamondYUp = 0.585;
float diamondYDown = 0.465;
float tntXLeft = -0.905;
float tntXRight = -0.73;
float tntYUp = -0.1;
float tntYDown = -0.26;
float diamondBlockXLeft = -0.31;
float diamondBlockXRight = -0.19;
float diamondBlockYUp = -0.43;
float diamondBlockYDown = -0.59;
float emeraldXLeft = -0.60;
float emeraldXRight = -0.50;
float emeraldYUp = 0.275;
float emeraldYDown = 0.125;
float craftingTableXLeft = -0.261;
float craftingTableXRight = -0.118;
float craftingTableYUp = 0.145;
float craftingTableYDown = -0.115;
float magmaXLeft = 0.87;
float magmaXRight = 0.97;
float magmaYUp = 0.70;
float magmaYDown = 0.60;
float ironXLeft = 0.26;
float ironXRight = 0.345;
float ironYUp = 0.24;
float ironYDown = 0.115;
float pumpkinXLeft = 0.057;
float pumpkinXRight = 0.197;
float pumpkinYUp = 0.97;
float pumpkinYDown = 0.77;
float furnaceXLeft = 0.633;
float furnaceXRight = 0.773;
float furnaceYUp = -0.0275;
float furnaceYDown = -0.135;
 

 

bool diamondFound = false;
bool testStarted = false;
bool diamondBlockFound = false;
bool emeraldFound = false;
bool craftingTableFound = false;
bool magmaFound = false;
bool ironFound = false;
bool pumpkinFound = false;
bool furnaceFound = false;
bool win = false;
bool check = false; //diamond block
bool check1 = false; //emerald
bool check2 = false; //furnace
bool check3 = false; //magma
bool check4 = false; //pumpkin
bool check5 = false; //tnt
bool check6 = false; //crafting table
bool check7 = false; //iron ore
bool check8 = false; //diamond ore
bool bulletShot = false;


float playerVelX = 0.0f;
float playerVelY = 0.0f;
float bulletPosX = 0.0f;
float bulletPosY = 0.0f;
float counter = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1200, 800, "Space Invaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// NEW SHADER
	Shader newshaderProgram("texture.vs", "texture.fs");
	Shader grayScaleProgram("texture.vs", "grayScale.fs");
	Shader playersProgram("players.fs", "players.vs");


	// Add vertex data
	float vertices[] = {
		//	X,		Y,		Z
		// Default Triangle
			-0.5f,	-0.5f,	0.0f,	// Middle Left 
			0.5f,	-0.5f,	0.0f,	// Middle Right
			0.0f,	0.5f,	0.0f,	// Top Center

	};

	float splashScreenVertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// VBO - Vertex Buffer Object - The indices into the VAO that define the shape you want


	VAO VAOSplash;
	VAOSplash.Bind();
	VBO VBOSplash(splashScreenVertices, sizeof(splashScreenVertices));
	EBO EBOSplash(indices, sizeof(indices));
	VAOSplash.LinkAttrib(VBOSplash, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOSplash.LinkAttrib(VBOSplash, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOSplash.LinkAttrib(VBOSplash, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));


	Texture splashScreen("titleScreen.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	glUniform1i(glGetUniformLocation(newshaderProgram.ID, "texure1"), 0);
	VAOSplash.Unbind();
	VBOSplash.Unbind();
	EBOSplash.Unbind();


	VAO VAOGame;
	VAOGame.Bind();
	VBO VBOGame(splashScreenVertices, sizeof(splashScreenVertices));
	EBO EBOGame(indices, sizeof(indices));
	VAOGame.LinkAttrib(VBOSplash, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOGame.LinkAttrib(VBOSplash, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOGame.LinkAttrib(VBOSplash, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture gameScreen("background.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

	VAO VAOTest;
	VAOTest.Bind();
	VBO VBOTest(splashScreenVertices, sizeof(splashScreenVertices));
	EBO EBOTest(indices, sizeof(indices));
	VAOTest.LinkAttrib(VBOTest, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOTest.LinkAttrib(VBOTest, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOTest.LinkAttrib(VBOTest, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture testScreen("after.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAO VAOWin;
	VAOWin.Bind();
	VBO VBOWin(splashScreenVertices, sizeof(splashScreenVertices));
	EBO EBOWin(indices, sizeof(indices));
	VAOWin.LinkAttrib(VBOWin, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOWin.LinkAttrib(VBOWin, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOWin.LinkAttrib(VBOWin, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture winScreen("endScreen.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);



	float playerVertices[] = {
		// positions          // colors           // texture coords
		 0.1f,  0.1f, 0.0f,   1.0f, 1.0f, // top right
		 0.1f,  0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.0f,  0.0f, 0.0f,    0.0f, 0.0f, // bottom left
		 0.0f,  0.1f, 0.0f,   0.0f, 1.0f  // top left 
	};

	VAOGame.Unbind();
	VBOGame.Unbind();
	EBOGame.Unbind();


	VAO VAOPlayer;
	VAOPlayer.Bind();
	VBO VBOPlayer(playerVertices, sizeof(playerVertices));
	EBO EBOPlayer(indices, sizeof(indices));
	VAOPlayer.LinkAttrib(VBOPlayer, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAOPlayer.LinkAttrib(VBOPlayer, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	Texture playerObject("ship.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	///Diamond
	VAOPlayer.Unbind();
	VBOPlayer.Unbind();
	EBOPlayer.Unbind();

	float diamondVertices[] = {
		// positions          // colors           // texture coords
		 diamondXRight,  diamondYUp, 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 diamondXRight, diamondYDown, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom right
		diamondXLeft, diamondYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		diamondXLeft,  diamondYUp, 0.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f  // top left 
	};


	VAO VAODiamond;
	VAODiamond.Bind();
	VBO VBODiamond(diamondVertices, sizeof(diamondVertices));
	EBO EBODiamond(indices, sizeof(indices));
	VAODiamond.LinkAttrib(VBODiamond, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAODiamond.LinkAttrib(VBODiamond, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAODiamond.LinkAttrib(VBODiamond, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	

	Texture diamondObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAODiamond.Unbind();
	VBODiamond.Unbind();
	EBODiamond.Unbind();

	//Tnt Object
	float tntVertices[] = {
		// positions          // colors           // texture coords
		 tntXRight, tntYUp, 0.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 tntXRight, tntYDown, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom right
		 tntXLeft,  tntYDown, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom left
		 tntXLeft,  tntYUp, 0.0f,   0.0f, 0.0f, 0.0f,  0.0f, 1.0f  // top left 
	};

	VAO VAOTnt;
	VAOTnt.Bind();
	VBO VBOTnt(tntVertices, sizeof(tntVertices));
	EBO EBOTnt(indices, sizeof(indices));
	VAOTnt.LinkAttrib(VBOTnt, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOTnt.LinkAttrib(VBOTnt, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOTnt.LinkAttrib(VBOTnt, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture tntObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOTnt.Unbind();
	VBOTnt.Unbind();
	EBOTnt.Unbind();

	//Bullet Object
	float bulletVertices[] = {
		// positions          // colors           // texture coords
		 // positions          // colors           // texture coords
		 0.075f,  0.15f, 0.0f,   1.0f, 1.0f, // top right
		 0.075f,  0.1f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.025f,  0.1f, 0.0f,    0.0f, 0.0f, // bottom left
		 0.025f,  0.15f, 0.0f,   0.0f, 1.0f  // top left 
	};

	VAOGame.Unbind();
	VBOGame.Unbind();
	EBOGame.Unbind();


	VAO VAOBullet;
	VAOBullet.Bind();
	VBO VBOBullet(bulletVertices, sizeof(bulletVertices));
	EBO EBOBullet(indices, sizeof(indices));
	VAOBullet.LinkAttrib(VBOBullet, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAOBullet.LinkAttrib(VBOBullet, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	Texture bulletObject("bullet.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOBullet.Unbind();
	VBOBullet.Unbind();
	EBOBullet.Unbind();

	//Diamond Block
	float diamondBlockVertices[] = {
		// positions          // colors           // texture coords
		 diamondBlockXRight, diamondBlockYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 diamondBlockXRight, diamondBlockYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 diamondBlockXLeft,  diamondBlockYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 diamondBlockXLeft,  diamondBlockYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAODiamondBlock;
	VAODiamondBlock.Bind();
	VBO VBODiamondBlock(diamondBlockVertices, sizeof(diamondBlockVertices));
	EBO EBODiamondBlock(indices, sizeof(indices));
	VAODiamondBlock.LinkAttrib(VBODiamondBlock, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAODiamondBlock.LinkAttrib(VBODiamondBlock, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAODiamondBlock.LinkAttrib(VBODiamondBlock, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture diamondBlockObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAODiamondBlock.Unbind();
	VBODiamondBlock.Unbind();
	EBODiamondBlock.Unbind();

	//Emerald
	float emeraldVertices[] = {
		// positions          // colors           // texture coords
		 emeraldXRight,  emeraldYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 emeraldXRight, emeraldYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		emeraldXLeft,  emeraldYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		emeraldXLeft,  emeraldYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOEmerald;
	VAOEmerald.Bind();
	VBO VBOEmerald(emeraldVertices, sizeof(emeraldVertices));
	EBO EBOEmerald(indices, sizeof(indices));
	VAOEmerald.LinkAttrib(VBOEmerald, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOEmerald.LinkAttrib(VBOEmerald, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOEmerald.LinkAttrib(VBOEmerald, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture emeraldObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOEmerald.Unbind();
	VBOEmerald.Unbind();
	EBOEmerald.Unbind();
	//Crafting Table
	float craftingTableVertices[] = {
		// positions          // colors           // texture coords
		 craftingTableXRight,  craftingTableYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 craftingTableXRight,  craftingTableYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		craftingTableXLeft,   craftingTableYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		craftingTableXLeft,   craftingTableYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCraftingTable;
	VAOCraftingTable.Bind();
	VBO VBOCraftingTable(craftingTableVertices, sizeof(craftingTableVertices));
	EBO EBOCraftingTable(indices, sizeof(indices));
	VAOCraftingTable.LinkAttrib(VBOCraftingTable, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCraftingTable.LinkAttrib(VBOCraftingTable, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCraftingTable.LinkAttrib(VBOCraftingTable, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture craftingTableObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCraftingTable.Unbind();
	VBOCraftingTable.Unbind();
	EBOCraftingTable.Unbind();
	//Magma vv
	float magmaVertices[] = {
		// positions          // colors           // texture coords
		 magmaXRight,  magmaYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 magmaXRight,  magmaYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		magmaXLeft,    magmaYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		magmaXLeft,    magmaYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOMagma;
	VAOMagma.Bind();
	VBO VBOMagma(magmaVertices, sizeof(magmaVertices));
	EBO EBOMagma(indices, sizeof(indices));
	VAOMagma.LinkAttrib(VBOMagma, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOMagma.LinkAttrib(VBOMagma, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOMagma.LinkAttrib(VBOMagma, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture magmaObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOMagma.Unbind();
	VBOMagma.Unbind();
	EBOMagma.Unbind();
	//Iron vv
	float ironVertices[] = {
		// positions          // colors           // texture coords
		  ironXRight,  ironYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		  ironXRight,  ironYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		  ironXLeft,  ironYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		  ironXLeft,  ironYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOIron;
	VAOIron.Bind();
	VBO VBOIron(ironVertices, sizeof(ironVertices));
	EBO EBOIron(indices, sizeof(indices));
	VAOIron.LinkAttrib(VBOIron, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOIron.LinkAttrib(VBOIron, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOIron.LinkAttrib(VBOIron, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture ironObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOIron.Unbind();
	VBOIron.Unbind();
	EBOIron.Unbind();

	//Pumpkin vv
	float pumpkinVertices[] = {
		// positions          // colors           // texture coords
		  pumpkinXRight,  pumpkinYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		  pumpkinXRight,  pumpkinYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		  pumpkinXLeft,  pumpkinYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		  pumpkinXLeft,  pumpkinYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOPumpkin;
	VAOPumpkin.Bind();
	VBO VBOPumpkin(pumpkinVertices, sizeof(pumpkinVertices));
	EBO EBOPumpkin(indices, sizeof(indices));
	VAOPumpkin.LinkAttrib(VBOPumpkin, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOPumpkin.LinkAttrib(VBOPumpkin, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOPumpkin.LinkAttrib(VBOPumpkin, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture pumpkinObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOPumpkin.Unbind();
	VBOPumpkin.Unbind();
	EBOPumpkin.Unbind();
	//Furnace vv
	float furnaceVertices[] = {
		// positions          // colors           // texture coords
		  furnaceXRight,  furnaceYUp, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		  furnaceXRight,  furnaceYDown, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		  furnaceXLeft,  furnaceYDown, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		  furnaceXLeft,  furnaceYUp, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	VAO VAOFurnace;
	VAOFurnace.Bind();
	VBO VBOFurnace(furnaceVertices, sizeof(furnaceVertices));
	EBO EBOFurnace(indices, sizeof(indices));
	VAOFurnace.LinkAttrib(VBOFurnace, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOFurnace.LinkAttrib(VBOFurnace, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOFurnace.LinkAttrib(VBOFurnace, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture furnaceObject("enemy1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOFurnace.Unbind();
	VBOFurnace.Unbind();
	EBOFurnace.Unbind();
	

	//check mark vv
	float checkVertices[] = { //diamond block
		// positions          // colors           // texture coords
		 -0.36f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 -0.36f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.58f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.58f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck;
	VAOCheck.Bind();
	VBO VBOCheck(checkVertices, sizeof(checkVertices));
	EBO EBOCheck(indices, sizeof(indices));
	VAOCheck.LinkAttrib(VBOCheck, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck.LinkAttrib(VBOCheck, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck.LinkAttrib(VBOCheck, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture checkObject("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck.Unbind();
	VBOCheck.Unbind();
	EBOCheck.Unbind();

	//check mark1 vv
	float check1Vertices[] = { //emerald
		// positions          // colors           // texture coords
		 -0.24f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 -0.24f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.46f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.46f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck1;
	VAOCheck1.Bind();
	VBO VBOCheck1(check1Vertices, sizeof(check1Vertices));
	EBO EBOCheck1(indices, sizeof(indices));
	VAOCheck1.LinkAttrib(VBOCheck1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck1.LinkAttrib(VBOCheck1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck1.LinkAttrib(VBOCheck1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check1Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck1.Unbind();
	VBOCheck1.Unbind();
	EBOCheck1.Unbind();

	//check mark2 vv
	float check2Vertices[] = { //furnace
		// positions          // colors           // texture coords
		 -0.11f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 -0.11f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.33f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.33f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck2;
	VAOCheck2.Bind();
	VBO VBOCheck2(check2Vertices, sizeof(check2Vertices));
	EBO EBOCheck2(indices, sizeof(indices));
	VAOCheck2.LinkAttrib(VBOCheck2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck2.LinkAttrib(VBOCheck2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck2.LinkAttrib(VBOCheck2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check2Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck2.Unbind();
	VBOCheck2.Unbind();
	EBOCheck2.Unbind();

	//check mark3 vv
	float check3Vertices[] = { //magma
		// positions          // colors           // texture coords
		 0.01f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.01f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.21f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.21f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck3;
	VAOCheck3.Bind();
	VBO VBOCheck3(check3Vertices, sizeof(check3Vertices));
	EBO EBOCheck3(indices, sizeof(indices));
	VAOCheck3.LinkAttrib(VBOCheck3, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck3.LinkAttrib(VBOCheck3, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck3.LinkAttrib(VBOCheck3, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check3Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck3.Unbind();
	VBOCheck3.Unbind();
	EBOCheck3.Unbind();

	//check mark4 vv
	float check4Vertices[] = { //pumpkin
		// positions          // colors           // texture coords
		 0.14f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.14f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.08f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.08f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck4;
	VAOCheck4.Bind();
	VBO VBOCheck4(check4Vertices, sizeof(check4Vertices));
	EBO EBOCheck4(indices, sizeof(indices));
	VAOCheck4.LinkAttrib(VBOCheck4, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck4.LinkAttrib(VBOCheck4, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck4.LinkAttrib(VBOCheck4, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check4Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck4.Unbind();
	VBOCheck4.Unbind();
	EBOCheck4.Unbind();

	//check mark5 vv
	float check5Vertices[] = { //tnt
		// positions          // colors           // texture coords
		 0.26f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.26f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.04f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 0.04f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck5;
	VAOCheck5.Bind();
	VBO VBOCheck5(check5Vertices, sizeof(check5Vertices));
	EBO EBOCheck5(indices, sizeof(indices));
	VAOCheck5.LinkAttrib(VBOCheck5, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck5.LinkAttrib(VBOCheck5, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck5.LinkAttrib(VBOCheck5, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check5Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck5.Unbind();
	VBOCheck5.Unbind();
	EBOCheck5.Unbind();

	//check mark6 vv
	float check6Vertices[] = { //crafting table
		// positions          // colors           // texture coords
		 0.39f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.39f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.17f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 0.17f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck6;
	VAOCheck6.Bind();
	VBO VBOCheck6(check6Vertices, sizeof(check6Vertices));
	EBO EBOCheck6(indices, sizeof(indices));
	VAOCheck6.LinkAttrib(VBOCheck6, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck6.LinkAttrib(VBOCheck6, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck6.LinkAttrib(VBOCheck6, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check6Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck6.Unbind();
	VBOCheck6.Unbind();
	EBOCheck6.Unbind();

	//check mark7 vv
	float check7Vertices[] = { //iron ore
		// positions          // colors           // texture coords
		 0.52f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.52f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.30f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 0.30f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck7;
	VAOCheck7.Bind();
	VBO VBOCheck7(check7Vertices, sizeof(check7Vertices));
	EBO EBOCheck7(indices, sizeof(indices));
	VAOCheck7.LinkAttrib(VBOCheck7, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck7.LinkAttrib(VBOCheck7, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck7.LinkAttrib(VBOCheck7, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check7Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck7.Unbind();
	VBOCheck7.Unbind();
	EBOCheck7.Unbind();

	//check mark8 vv
	float check8Vertices[] = { //diamond ore
		// positions          // colors           // texture coords
		 0.64f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.64f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.42f, -1.00f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 0.42f,  -0.60f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
	};


	VAO VAOCheck8;
	VAOCheck8.Bind();
	VBO VBOCheck8(check8Vertices, sizeof(check8Vertices));
	EBO EBOCheck8(indices, sizeof(indices));
	VAOCheck8.LinkAttrib(VBOCheck8, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAOCheck8.LinkAttrib(VBOCheck8, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAOCheck8.LinkAttrib(VBOCheck8, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Texture check8Object("cannon.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	VAOCheck8.Unbind();
	VBOCheck8.Unbind();
	EBOCheck8.Unbind();

	//timer in console window 
	
	double time = 0.0;
	double lastTime = 0.0;
	std::cout << "Time in seconds" << std::endl;
	std::cout << "---------------" << std::endl;
	
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		newshaderProgram.Activate();
		if (!grayScale) { //include to juola
			newshaderProgram.Activate();
		}
		else {
			grayScaleProgram.Activate();
		}
		splashScreen.Bind();
		splashScreen.Activate(); //include to juola
		//glBindVertexArray(VAO);

		VAOSplash.Bind();


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		if (gameStarted == true) {
			gameScreen.Bind();
			gameScreen.Activate();
			VAOSplash.Bind();
			//glfwSetTime(0.0);
			//void glfwSetTime(double 	time);
			
			time = glfwGetTime();
			if (time - lastTime > 1) {
				lastTime = time;
				lastTime = trunc(lastTime);
				std::cout << time << std::endl;
			}
			else if (time < 0.01) {
				lastTime = 0.00;
				time = 0.00;
			}
			

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			if (playerFound == false) {
				glm::mat4 transform = glm::mat4(1.0f);
				transform = glm::translate(transform, glm::vec3(playerVelX, playerVelY, 0.0f));
				playersProgram.Activate();
				unsigned int transformLoc = glGetUniformLocation(playersProgram.ID, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
				playerObject.Bind();
				playerObject.Activate();
				VAOPlayer.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				newshaderProgram.Activate();
			}
			if (bulletShot) {
				counter = counter + 0.01f;
				glm::mat4 transform = glm::mat4(1.0f);
				transform = glm::translate(transform, glm::vec3(bulletPosX, bulletPosY+counter, 0.0f));
				playersProgram.Activate();
				unsigned int transformLoc = glGetUniformLocation(playersProgram.ID, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
				bulletObject.Bind();
				bulletObject.Activate();
				VAOBullet.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				newshaderProgram.Activate();
			}
			
			if (diamondFound == false) {
				diamondObject.Bind();
				diamondObject.Activate();
				VAODiamond.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (tntFound == false) {
				tntObject.Bind();
				tntObject.Activate();
				VAOTnt.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (diamondBlockFound == false) {
				diamondBlockObject.Bind();
				diamondBlockObject.Activate();
				VAODiamondBlock.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (emeraldFound == false) {
				emeraldObject.Bind();
				emeraldObject.Activate();
				VAOEmerald.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (craftingTableFound == false) {
				craftingTableObject.Bind();
				craftingTableObject.Activate();
				VAOCraftingTable.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (magmaFound == false) {
				magmaObject.Bind();
				magmaObject.Activate();
				VAOMagma.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (ironFound == false) {
				ironObject.Bind();
				ironObject.Activate();
				VAOIron.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (pumpkinFound == false) {
				pumpkinObject.Bind();
				pumpkinObject.Activate();
				VAOPumpkin.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (furnaceFound == false) {
				furnaceObject.Bind();
				furnaceObject.Activate();
				VAOFurnace.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			
			if (check == true) {
				checkObject.Bind();
				checkObject.Activate();
				VAOCheck.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check1 == true) {
				check1Object.Bind();
				check1Object.Activate();
				VAOCheck1.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check2 == true) {
				check2Object.Bind();
				check2Object.Activate();
				VAOCheck2.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check3 == true) {
				check3Object.Bind();
				check3Object.Activate();
				VAOCheck3.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check4 == true) {
				check4Object.Bind();
				check4Object.Activate();
				VAOCheck4.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check5 == true) {
				check5Object.Bind();
				check5Object.Activate();
				VAOCheck5.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check6 == true) {
				check6Object.Bind();
				check6Object.Activate();
				VAOCheck6.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check7 == true) {
				check7Object.Bind();
				check7Object.Activate();
				VAOCheck7.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			if (check8 == true) {
				check8Object.Bind();
				check8Object.Activate();
				VAOCheck8.Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
		else if (testStarted == true) {
			testScreen.Bind();
			testScreen.Activate();
			VAOTest.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else if (win == true) {
			winScreen.Bind();
			winScreen.Activate();
			VAOWin.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapInterval(1);
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	glfwTerminate();
	return 0;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	int width, height;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &width, &height);
	//std::cout << "xPos: " << xpos << " yPos: " << ypos << std::endl;


	xNormalized = xpos / width;
	xNormalized = ((xpos - width / 2) / (width / 2));
	//(pos - scr width / 2 ) / (scren wid * 2)
	yNormalized = ypos / height;
	yNormalized = ((ypos - height / 2) / (height / 2)) * -1;
	if (action == GLFW_PRESS)
		std::cout << "xNormalized : " << xNormalized << " yNormalized : " << yNormalized << std::endl;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { //include to juola
		if (grayScale) {
			grayScale = false;
		}
		else {
			grayScale = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		playerVelX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		playerVelX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		playerVelY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		playerVelY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bulletShot) {
		bulletShot = true;
		bulletPosY = playerVelY;
		bulletPosX = playerVelX;
		counter = 0.0f;
	}
	float bulletPosYTotal = bulletPosY + counter;
	// Bullet Collision
	if ((bulletPosX >= diamondXLeft && bulletPosX <= diamondXRight) && (bulletPosYTotal >= diamondYDown && bulletPosYTotal <= diamondYUp)) 
	{
		bulletShot = false;
		diamondFound = true;
		check2 = true;
	}
	if ((bulletPosX >= tntXLeft && bulletPosX <= tntXRight) && (bulletPosYTotal >= tntYDown && bulletPosYTotal <= tntYUp))
	{
		bulletShot = false;
		tntFound = true;
		check5 = true;
	}
	if ((bulletPosX >= diamondBlockXLeft && bulletPosX <= diamondBlockXRight) && (bulletPosYTotal >= diamondBlockYDown && bulletPosYTotal <= diamondBlockYUp))
	{
		bulletShot = false;
		diamondBlockFound = true;
		check = true;
	}
	if ((bulletPosX >= emeraldXLeft && bulletPosX <= emeraldXRight) && (bulletPosYTotal >= emeraldYDown && bulletPosYTotal <= emeraldYUp))
	{
		bulletShot = false;
		emeraldFound = true;
		check1 = true;
	}
	if ((bulletPosX >= craftingTableXLeft && bulletPosX <= craftingTableXRight) && (bulletPosYTotal >= craftingTableYDown && bulletPosYTotal <= craftingTableYUp))
	{
		bulletShot = false;
		craftingTableFound = true;
		check6 = true;
	}
	if ((bulletPosX >= magmaXLeft && bulletPosX <= magmaXRight) && (bulletPosYTotal >= magmaYDown && bulletPosYTotal <= magmaYUp))
	{
		bulletShot = false;
		magmaFound = true;
		check3 = true;
	}
	if ((bulletPosX >= ironXLeft && bulletPosX <= ironXRight) && (bulletPosYTotal >= ironYDown && bulletPosYTotal <= ironYUp))
	{
		bulletShot = false;
		ironFound = true;
		check7 = true;
	}
	if ((bulletPosX >= pumpkinXLeft && bulletPosX <=pumpkinXRight) && (bulletPosYTotal >= pumpkinYDown && bulletPosYTotal <= pumpkinYUp))
	{
		bulletShot = false;
		pumpkinFound = true;
		check4 = true;
	}
	if ((bulletPosX >= furnaceXLeft && bulletPosX <= furnaceXRight) && (bulletPosYTotal >= furnaceYDown && bulletPosYTotal <= furnaceYUp))
	{
		bulletShot = false;
		furnaceFound = true;
		check2 = true;
	}

	// Bullet Boundary
	if (bulletPosYTotal > 1.0f) {
		bulletShot = false;
		counter = 0.0f;
	}
	//Boundary
	if (playerVelX > (1.0f - .1f)) {
		playerVelX = (0.9f);
	}
	else if (playerVelX < -1.0f) {
		playerVelX = -1.0f;
	}
	if (playerVelY > (1.0f - .1f)) {
		playerVelY = (0.9f);
	}
	else if (playerVelY < -1.0f) {
		playerVelY = -1.0f;
	}

	if (xNormalized >= -0.154 && xNormalized <= 0.145 && yNormalized >= 0.08 && yNormalized <= 0.165 && gameStarted == false) { //start screen - play
		gameStarted = true;
		//std::cout << "test" << std::endl;
		glfwSetTime(0.00);
	}
	if (xNormalized >= -0.154 && xNormalized <= 0.145 && yNormalized >= -0.0875 && yNormalized <= 0.0125 && gameStarted == false) { //start screen - quit
		glfwSetWindowShouldClose(window, true);
	}
	
	/*else if (((xNormalized > 0.9 && xNormalized <= 1.0) && (yNormalized > 0.9 && yNormalized <= 1.0)) && gameStarted == false) { //test
		testStarted = true;

		gameStarted = false;
		//isFound = false;
	}*/
	else if (((xNormalized >= tntXLeft && xNormalized <= tntXRight) && (yNormalized >= tntYDown && yNormalized <= tntYUp)) && gameStarted == true) {
		tntFound = true;
		check5 = true;
	}
	else if (((xNormalized >= diamondXLeft && xNormalized <= diamondXRight) && (yNormalized >= diamondYDown && yNormalized <= diamondYUp)) && gameStarted == true) {
		diamondFound = true;
		check8 = true;
	}
	else if (((xNormalized >= diamondBlockXLeft && xNormalized <= diamondBlockXRight) && (yNormalized >= diamondBlockYDown && yNormalized <= diamondBlockYUp)) && gameStarted == true) {
		diamondBlockFound = true;
		check = true;
	}
	else if (((xNormalized >= emeraldXLeft && xNormalized <= emeraldXRight) && (yNormalized >= emeraldYDown && yNormalized <= emeraldYUp)) && gameStarted == true) {
		emeraldFound = true;
		check1 = true;
	}
	else if (((xNormalized >= craftingTableXLeft && xNormalized <= craftingTableXRight) && (yNormalized >= craftingTableYDown && yNormalized <= craftingTableYUp)) && gameStarted == true) {
		craftingTableFound = true;
		check6 = true;
	}
	else if (((xNormalized >= magmaXLeft && xNormalized <= magmaXRight) && (yNormalized >= magmaYDown && yNormalized <= magmaYUp)) && gameStarted == true) {
		magmaFound = true;
		check3 = true;
	}
	else if (((xNormalized >= ironXLeft && xNormalized <= ironXRight) && (yNormalized >= ironYDown && yNormalized <= ironYUp)) && gameStarted == true) {
		ironFound = true;
		check7 = true;
	}
	else if (((xNormalized >= pumpkinXLeft && xNormalized <= pumpkinXRight) && (pumpkinYDown >= ironYDown && yNormalized <= pumpkinYUp)) && gameStarted == true) {
		pumpkinFound = true;
		check4 = true;
	}
	else if (((xNormalized >= furnaceXLeft && xNormalized <= furnaceXRight) && (yNormalized >= furnaceYDown && yNormalized <= furnaceYUp)) && gameStarted == true) {
		furnaceFound = true;
		check2 = true;
	}
	if (tntFound == true && diamondFound == true && diamondBlockFound == true && emeraldFound == true && craftingTableFound == true && magmaFound == true && ironFound == true && pumpkinFound == true && furnaceFound == true) {
		gameStarted = false;
		win = true;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}