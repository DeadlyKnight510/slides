#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <string>
#include <cmath>
#include <vector>
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
using namespace cv;
using namespace std;

String sponsors[] = {"bronze/Senix-logo.png","bronze/ashcroft.png","bronze/impulse.png","bronze/intel.png","bronze/ptsa.png","bronze/rugged.png","bronze/teco.png", "silver/Danville.png","silver/pni.png","silver/pointgrey.png","silver/rotary.png","silver/sexton-logo.png","silver/teledynebenthos.png","silver/theia.png","silver/truevalue.png","silver/videoray.png", "gold/inverse.png", "gold/pleasantontool.png", "platinum/3daccess.png","platinum/NIOfficialLogo.png","platinum/analogdevices1.png","platinum/andreesenhororwitz.png", "platinum/atomikrc.png","platinum/datron.png", "platinum/ebay.png", "platinum/google.png","platinum/subconn.png","platinum/svcf.png","platinum/tanius-logo-web.png"};
int level[] ={1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4};
int border = 50;

Mat slide(Mat first, Mat second, int x)
{
	Mat between(400,800,CV_8UC3,Scalar(255,255,255));

	for(int o=799; o >= x; o--)
	{
		first.col(o).copyTo(between.col(o-x));
	}
	if(x!=0)
	{
		for(int e=0; e <x; e++)
		{
			second.col(e).copyTo(between.col(between.cols-x+e));
		}
	}

	return between;	
}
Mat addBG(Mat input, Mat overlay)
{
	for(int j = 0; j <border-20; j++)
	{
		overlay.col(j).copyTo(input.col(j));
	}
	for(int c = 799; c >800-(border-20); c--)
	{
		overlay.col(c).copyTo(input.col(c));
	}
	for(int b = 0; b <border-20; b++)
	{
		overlay.row(b).copyTo(input.row(b));
	}
	for(int a = 399; a >400-(border-20); a--)
	{
		overlay.row(a).copyTo(input.row(a));
	}
	return input;
}
Mat changeSize(Mat src, int width, int height)
{
	Mat output (400,800,CV_8UC3, Scalar(255,255,255));

	float rows = src.rows;
	float cols = src.cols;
	float scale = 1;
	int newX,newY;

	if(cols/rows>width/height)
	{
		scale = width/cols;
		newX = cols * scale;
		newY = rows * scale;

		resize(src, src, cvSize(newX,newY));
		src.copyTo(output( cv::Rect(border, ((400-border*2)-src.rows)/2+border, src.cols, src.rows)));
	}
	else
	{
		scale = height/rows;
		newX = cols * scale;
		newY = rows * scale;

		resize(src, src, cvSize(newX,newY));
		src.copyTo(output( cv::Rect(((800-border*2)-src.cols)/2+border, border, src.cols, src.rows)));
	}

	return output;
}

int main( int argc, char** argv )
{
	Mat src, nextSrc, output, nextOutput,bg, bg2;
	int width = 800-border*2;
	int height = 400-border*2;
	String name;
	String name2;

	Mat bronze = imread("img/bronze2.png", CV_LOAD_IMAGE_COLOR); 
	Mat silver = imread("img/silver2.png", CV_LOAD_IMAGE_COLOR); 
	Mat gold = imread("img/gold2.png", CV_LOAD_IMAGE_COLOR); 
	Mat platinum = imread("img/platinum2.png", CV_LOAD_IMAGE_COLOR); 

	resize(bronze, bronze, Size(800,400)); 
	resize(silver, silver, Size(800,400)); 
	resize(gold, gold, Size(800,400)); 
	resize(platinum, platinum, Size(800,400)); 

	String folderpath;
	vector<String> filenames;
	while(true)
	{
		folderpath = "img/platinum/*.png";
		cv::glob(folderpath, filenames);
		for(int f=0; f<4; f++)
		{
			switch(f) {
				case 1: folderpath= "img/gold/*.png";break;
				case 2: folderpath= "img/silver/*.png";break;
				case 3: folderpath= "img/bronze/*.png";break;
				case 0: folderpath= "img/platinum/*.png";break;
			}
			cv::glob(folderpath, filenames);

			for (size_t i=0; i<filenames.size(); i++)
			{
				src = imread(filenames[i]);
				name = filenames[i];
		
				if((i+1)==filenames.size())
				{
					switch(f) {
						case 0: name2 = "img/gold/inverse.png" ;break;
						case 1: name2= "img/silver/Danville.png";break;
						case 2: name2= "img/bronze/Senix-logo.png";break;
						case 3: name2= "img/platinum/3daccess.png";break;
					}
					nextSrc = imread(name2);

				}
				else
				{
					nextSrc = imread(filenames[i+1]);
					name2 = filenames[i+1];
				}

				for(int w = 0; w < ARRAY_SIZE(sponsors); w++)
				{
					if(name=="img/" + sponsors[w])
					{
						int color = level[w];
						switch(color) {
							case 1: bronze.copyTo(bg); break;
							case 2: silver.copyTo(bg); break;
							case 3: gold.copyTo(bg); break;
							case 4: platinum.copyTo(bg); break;
						}
					}
				}
				for(int q = 0; q < ARRAY_SIZE(sponsors); q++)
				{
					if(name2=="img/" + sponsors[q])
					{
						int color = level[q];
						switch(color) {
							case 1: bronze.copyTo(bg2); break;
							case 2: silver.copyTo(bg2); break;
							case 3: gold.copyTo(bg2); break;
							case 4: platinum.copyTo(bg2); break;
						}
					}
				}

				output = addBG(changeSize(src, width, height),bg);
				nextOutput = addBG(changeSize(nextSrc, width, height),bg2);

				namedWindow("Sponsors!",0);
				imshow( "Sponsors!", output );
				waitKey(5000);

				for(int k=0; k <= 800; k=k+k*(801-k)*0.00005+1)
				{
					namedWindow("Sponsors!",0);
					imshow( "Sponsors!", slide(output, nextOutput, k));
					waitKey(1);	
				}
			}
		}
	}
	return(0);
}
