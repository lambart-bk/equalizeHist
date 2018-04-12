#include "equalizehist/equalizehist.h"
#include"iostream"
#include"iomanip"
using namespace std;
using namespace cv;
void printHist(int* hist,int count=256)
{
  size_t sum=0;
  for(int i=0;i<count;i++)
  {
    if(i%16==0)
      cout<<endl;
    cout<<setw(5)<<hist[i];
    sum+=hist[i];
  }
  cout<<endl<<sum<<endl;
  cout<<endl;
}
int main(int argc,char**argv)
{
  if(argc<2)
  {
    cout<<"usage: "<<argv[0]<<" path_to_image"<<endl;
    return 0;
  }
  
  Mat img=cv::imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  cout<<"image rows: "<<img.rows<<endl;
  cout<<"image cols: "<<img.cols<<endl;
  cout<<"image channels: "<<img.channels()<<endl;
  namedWindow("img");
  imshow("img",img);
  cvWaitKey(0);
  
  
  int hist[256]={0,},lut[256]={0,};
  //calculate the histogram for img_src
  for(size_t i=0;i<img.rows;i++)
    for(size_t j=0;j<img.cols;j++)
    {
      unsigned char *p_row=img.ptr<unsigned char>(i);
      CV_Assert(img.channels()==1);
      //cout<<" "<<(int)p_row[j]<<endl;
      int value=(int )p_row[j];
      CV_Assert(value>=0&&value<256);
      hist[value]+=1;
    }
  printHist(hist);
  
  //normalize the histogram
  int i=0;
  while(!hist[i]) i++;
  cout<<"img.total(): "<<img.total()<<endl;
  double scale=255.f/(img.total()-hist[i]);
  cout<<"scale: "<<scale<<endl;
  
  //compute the integral of the histogram
  int sum=0;
  for(lut[i++]=0;i<256;i++)
  {
    sum+=hist[i];
    lut[i]=saturate_cast<uchar>(sum * scale);
  }
/*  
 * implement by myself,it's wrong!!!
 double normhist[256];
 for(int i=0;i<256;i++)
    normhist[i]=255*hist[i]/(img.cols*img.rows);
  for(int i=0;i<256;i++)
  {
    double sum=0;
    for(int j=0;j<=i;j++)
      sum+=normhist[j];
    lut[i]=(int)sum;
  }*/

  printHist(lut);
  
  Mat officeHist;
  equalizeHist(img,officeHist);
  //transform the image,will normalize the brightness and increase the contrast of the image
  for(size_t i=0;i<img.rows;i++)
    for(size_t j=0;j<img.cols;j++)
    {
      unsigned char *p_rows=img.ptr<unsigned char>(i);
      p_rows[j]=lut[p_rows[j]];
    }
  imshow("img",img);
  cvWaitKey(0);
  //compare the result with office implement in opencv
  for(size_t i=0;i<img.rows;i++)
    for(size_t j=0;j<img.cols;j++)
    {
      unsigned char *p_imgrows=img.ptr<unsigned char>(i);
      unsigned char *p_oferows=officeHist.ptr<unsigned char>(i);
      if(p_imgrows[j]!=p_oferows[j])
	cout<<"error"<<endl;
    }
  cvDestroyAllWindows();
  
  return 0;
}
