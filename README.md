# Creating a Flowchart from a Dependency List
A work by: Alison Langer and Logan Munoz

This project finds the optimal course flow for a major with given constraints. This program will take a list of classes required to complete a major as its input and will produce a course flow of the order the classes should be taken. There are two possibilities for this output. The first is having no constraints, the second having a maximum credit value and a predetermined starting quarter. Based on these constraints, the program will output a different course flow that will allow completion of the major. The animation of the program output will demonstrate the progress of the classes taken towards completing the major. For this project we chose two majors: Computer Science and Exercise Science.

## Description
For this project we chose to create our program in C++. The program begins by asking the user to choose a file containing the required courses to complete a major. Then, the format of the file is validated as the file is parsed. As each line is parsed, a Course object is created containing the details for the course. A vertex is also created at this time and added to the adjacency list that stores the information for the graph. After every class has been added, the graph's edges are then created to demonstrate the connections between the classes and their prerequisites. After the graph has been created, the user is prompted for their maximum credits that they can take in a quarter and their starting quarter. Finally, the graphs are displayed both textually and as an animated picture for each output. 

## Requirements
**Note:** after reading these requirements, if you wish to download all the requirements at the same time to avoid several commands, you can use the command below to do so! If there are any that you already have installed on Ubuntu (MUST have Ubuntu set up first), then simply omit those from the list of packages following "install".
```
sudo apt-get install libboost-all-dev g++ graphviz xdg-utils imagemagick
```


The requirements to run this program are as follows:
 - Ubuntu
    - This is a subsystem that allows the user to run commands in a Linux command line. The instructions to download and set up your Ubuntu environment can be found [here](https://ubuntu.com/download/desktop "Ubuntu Setup"). 
    - Note that this application was developed and tested in an Ubuntu environment (specifically version 18.04, and may require some extra setup if you wish to use a different IDE.
- Boost C++ Libraries
    - These libraries are what contain many functions and data structures that this application utilizes, and thus are necessary to have downloaded in your Ubuntu environment to allow the header files to properly link.
    - The simplest way to get these downloaded is to enter the following command into your Ubuntu environment from the base directory. You may be prompted to enter your password and confirm several things along the way, but once the download is complete you are all set!
    - If you would like more information on Boost, or to see other ways of downloading it, feel free to investigate more [here](https://www.boost.org/ "Boost Info").
```
sudo apt-get install libboost-all-dev
```
- g++ / gcc Compiler
    - This is a compiler for C++ that will allow you to compile C++ code into an executable. Note that g++ version 7.5.0 was used in the creation of this project, which supports at least through C++17. This project requires at least C++11 to run, so there should be no extra work needed to match versions between the compiler and the code.
    - The following line of code should be entered into the commandline to get this set up, and as with the previous install will likely ask you for a password and to confirm several things along the way. For more information on the g++ compiler, check out their website [here](https://gcc.gnu.org/ "GCC Info").
```
sudo apt-get install g++
```
- graphviz
    - This is a supporting package that allows us to create a graphical visualization of the course schedule. Specifically, this allows us to convert .dot files into .png files so that you can view your schedule as an actual graph instead of just text!
    - Simply enter the following line of code, and as with the previous install will likely ask you for a password and to confirm several things along the way. If you want to learn more about what graphviz can do, feel free to check out their website [here](https://www.graphviz.org/ "Graphviz Info").
```
sudo apt-get install graphviz
```
- xdg-utils (*Optional*)
    - This is a package that will allow you to view images through your Ubuntu terminal. Use of this package is optional if you don't care to view the graphical representation of your schedule. However, if you wish to do so, this will allow you to see the image that is created for you!
    - To get this set up, use the line of code below in your Ubuntu terminal, and once more as with the previous install will likely ask you for a password and to confirm several things along the way. To read more about xdg-utils, take a look at their website [here](https://www.freedesktop.org/wiki/Software/xdg-utils/ "xdg Info").
```
sudo apt-get install xdg-utils
```
- ImageMagick (*Optional*)
    - This package is also optional, needed only if you wish to view the animated gif of your schedule being formed. If you don't want to see this animation, then feel free to skip this requirement.
    - To get this set up, use the line of code below in your Ubuntu terminal. As with the previous installation will likely ask you for a password and to confirm several things along the way. To read more about ImageMagick, take a look at their website [here](https://imagemagick.org/index.php "ImageMagick Info").
```
sudo apt-get install imagemagick
```
- C++11 (at least)
    - As mentioned before, this project requires at least C++11 to run. However, as long as you are using the g++ compiler, or a similar compiler in a different IDE, there should be no problems with the specific version.


## User Manual
**Assuming you have viewed and followed the instructions in the [Requirements](#requirements) section, running this program should be easy!** 
1. ***Clone this repository:***  It is assumed you know how to do this already, but if not, feel free to check out [this guide](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository "Repository Cloning Command Line") for a step-by-step on how to do that in the command line, or [this guide](https://docs.github.com/en/desktop/contributing-and-collaborating-using-github-desktop/adding-and-cloning-repositories/cloning-a-repository-from-github-to-github-desktop "Repository Cloning Desktop") for how to do it using GitHub Desktop.

2. ***Install required libraries:*** If you have not already done so after reading the requirements section, please install all external libraries and tools needed to run this program. This can easily be done by entering the following command into your Ubuntu terminal, and omitting any of these libraries which you already have downloaded. This same command can be found at the top of the [Requirements](#requirements) section. You may be asked to say "Y" a few times to allow the libraries to be downloaded as this command runs.
```
sudo apt-get install libboost-all-dev g++ graphviz xdg-utils imagemagick
```

3. ***Navitage to the repository directory:***  This can be done using the "cd" command followed by a space, and then the path to your repository. It could look something like this: "cd myDevelopmentFolder/myNewRepository".

4. ***Compile the cpp files:***  Once you have navigated to the correct directory, simply copy the command below into your Ubuntu terminal and press enter. This command will use the g++ compiler to compile the 4 files necessary (main.cpp, Course.cpp, ClassesSorter.cpp, and Display.cpp), and create an executable named "runProgram". Especially if this is the first time using the g++ compiler in this Ubuntu session, this may take a few seconds to build everything.  
```
g++ main.cpp Course.cpp ClassesSorter.cpp Display.cpp -o runProgram
```
5. ***Run the program:***  Once we have that executable, simply enter the following line of code, and the program will start running! You will be prompted to enter a text file to read from. Be sure to read what the program is telling you, as it will walk you through the rest of the steps and options available. Have fun!
```
./runProgram
```

## Reflection
This section will explain the methodology used to create the flowchart with the best time complexity that we could accomplish. To begin the program, we began by parsing the input file chosen by the user to both read in the given information as well as validating the file format in O(n<sup>2</sup>) time. This created objects for each of the classes and added them to the general graph as vertices. Then we used the information given by the courses and their prerequisites to create edges on the general graph by connecting each prerequisite to its sequence of classes in O(n<sup>3</sup>) time. Next the user is prompted to input the maximum credits and starting quarter constraints in O(1) time. 

After all of the information is gathered, the classes are sorted into the ideal course flow and scheduled by using the ClassesSorter class which overall is O(n<sup>2</sup>(m + n)) time. ClassesSorter first finds courses with no prerequisites or with their prerequisites taken in O(n) time, and then uses an iterative version of Depth First Search, O(m + n), to iterate through courses of increasing level (1000, 2000, 3000, etc.). However, those two are done independently of each other, so the overal time complexity of that portion stays at O(m + n). For the no constraints case, the course is simply added to a deque, so there is no increase in time complexity. However, for the constraints case, the course is scheduled using another method, ScheduleWishConstraints. That method runs in O(n<sup>2</sup>) time in a worst case scenario. The method loops until the course is scheduled, which may be O(n) if all other courses are scheduled with a low credit maximum such that there is effectively one course per year. Within that loop, the line with the largest time complexity is when a check is done to see if the credit maximum of the given quarter has been reached. In a situation where all other courses are scheduled on the same quarter, this would have to loop through all n - 1 of those courses, meaning the overall operation is O(n) worst case. Thus, the ScheduleWithConstraints is O(n<sup>2</sup>), making the DFS which calls this method O(n<sup>2</sup>(m + n)). When the schedules are created they are output as text with no constraints in O(n) time or with constraints in O(n<sup>2</sup>) time. 

Then, the Display class is used to create the pictorial representation of the course flow in O(n<sup>4</sup>) time. This includes creating individual .dot files for each quarter. Each .dot file includes the initialization of each class object with its corresponding subject color, edge data to show prerequisites and rankings to keep each set of classes in the correct column for each quarter. Each file describes the cumulative course flow for each quarter with its corresponding edges showing prerequisites for each class. Finally, the .dot files are converted into .png files and compiled into a single GIF that animates the entire course flow which is accomplished in O(n<sup>2</sup>) time. 

Overall, this project was challenging in many ways. One main challenge was learning how to use third party libraries. We often knew what we wanted the program to do, but utilizing those external libraries which employ new techniques in C++ (such as iterators) that we haven't seen before was difficult. It took some time to learn how to install and use the libraries functions. Another challenge was learning how to use the DOT language to create .dot files to show the course flow. Neither of us had used the DOT language before so this was new to us. The pictorial output had several challenges including how to group the classes by quarter and how to keep the ranking consistent so the classes stayed in their corresponding column. The solution that we used was to add “invisible” summer classes for each summer quarter that did not have classes. This way, they acted as placeholders to keep the rankings consistent. We also found that creating the GIF was challenging due to the fact that each of the .png files were created in different sizes depending on the amount of classes that had already been taken. We also needed to learn how to convert the .dot files to .png format from the program without the user’s input and how to combine the multiple .png files into a single animated GIF. Despite these difficulties, after many test programs and hours put into working with these new tools, we were able to use them efficiently to create a project we are proud of.

## Results
This section will include some screenshots of the program running so you can see what everything looks like before choosing to set this up! If you would like to see everything happening in real time, check out [this video](https://youtu.be/glKszGwu_r4 "Youtube Link") to see a YouTube video of the program running.


- Opening prompt and data entry:
![User Input](https://user-images.githubusercontent.com/59589283/159179011-92d2ee91-1c2e-443c-bb7f-eeaed5b6f329.JPG)

- Text output for graph without constraints:
![Text No Constraints](https://user-images.githubusercontent.com/59589283/159179032-ea44d96c-da41-49fa-9754-818a587e19cc.JPG)

- Text output for graph with constraints:
![Text With Constraints](https://user-images.githubusercontent.com/59589283/159179040-578209cb-d8b4-4afc-b74c-e1a875dd5989.JPG)

- Prompt for viewing the graphical output:
![Graphical Representation Prompt](https://user-images.githubusercontent.com/59589283/159186115-c7ab3fdf-fa54-4752-bbc1-14f3814c727f.JPG)

- Examples of the graphical outputs, for no constraints and constraints respectively:

![noConstraintsSchedule](https://user-images.githubusercontent.com/59589283/159185769-4ebe3f96-69f6-4e70-aee4-b2990ef0f705.jpg)
![mySchedule](https://user-images.githubusercontent.com/59589283/159179053-a2b3c372-59cf-4b74-a7a1-80bab38556e4.jpg)

- Prompt for viewing the animation:
![Animation Prompt](https://user-images.githubusercontent.com/59589283/159179062-c5dd630c-ad3b-4354-9f70-c960b8cf710c.JPG)

- Example animation:
![scheduleAnimation](https://user-images.githubusercontent.com/59589283/159179065-69f1d93b-90d7-46e3-9802-4b5a0f49122b.gif)


