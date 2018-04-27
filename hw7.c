#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS 400
#define COLS 400

#define sqr(x)	((x)*(x))

void header( int row, int col, unsigned char head[32] );
void getImage(double vector_s[3], double r, double a, double m, char filename[50]);

int main( int argc, char** argv )
{	
	char			filename[9][50];
	double    S[3];
	
	/* Image filenames */
	strcpy(filename[0], "image1.ras");
	strcpy(filename[1], "image2.ras");
	strcpy(filename[2], "image3.ras");
	strcpy(filename[3], "image4.ras");
	strcpy(filename[4], "image5.ras");
	strcpy(filename[5], "image6.ras");
	strcpy(filename[6], "image7.ras");
	strcpy(filename[7], "image8.ras");
	strcpy(filename[8], "image9.ras");
 
   
    
	/* Source direction S */
  S[0] = 0;
  S[1] = 0;
  S[2] = 1;
  getImage(S, 50, 0.5, 1, filename[0]);
  getImage(S, 10, 0.5, 1, filename[3]);
  getImage(S, 100, 0.5, 1, filename[4]);
  getImage(S, 50, 0.1, 1, filename[5]);
  getImage(S, 50, 1, 1, filename[6]);
  getImage(S, 50, 0.5, 0.1, filename[7]);
  getImage(S, 50, 0.5, 10000, filename[8]);
 
  S[0] = 1/sqrt(3);
  S[1] = 1/sqrt(3);
  S[2] = 1/sqrt(3);
  getImage(S, 50, 0.5, 1, filename[1]);
  
  S[0] = 1;
  S[1] = 0;
  S[2] = 0;
  getImage(S, 50, 0.5, 1, filename[2]);
    
	return 0;
}

void getImage(double vector_s[3], double r, double a, double m, char filename[50]){

  unsigned char	image[ROWS][COLS], head[32];
  double			theta,alpha,Ll,Ls,L;
  
  
  int				i,j,k,x,y;
	double			vector_n[3], vector_h[3], image_temp[ROWS][COLS];
  double      vector_v[3];
  double      max;
	
	FILE*			fp;
 
  header(ROWS, COLS, head);
   
  for ( i = 0 ; i < ROWS ; i++ )
  		for ( j = 0 ; j < COLS ; j++ ){
        image[i][j] = 0;
        image_temp[i][j] = 0;
  }
  
  /* Viewing direction V(x, y, z) */
    vector_v[0] = 0;
    vector_v[1] = 0;
    vector_v[2] = 1;
    
  /* Calculate angular bisector H */
  	vector_h[0] = vector_v[0] + vector_s[0];
  	vector_h[1] = vector_v[1] + vector_s[1];
  	vector_h[2] = vector_v[2] + vector_s[2];
  	double mag_h = sqrt(sqr(vector_h[0]) + sqr(vector_h[1]) + sqr(vector_h[2]));
  	vector_h[0] = vector_h[0]/mag_h;
  	vector_h[1] = vector_h[1]/mag_h;
  	vector_h[2] = vector_h[2]/mag_h;
  
  	
  	for(y=0;y<ROWS;y++){
  		for(x=0;x<COLS;x++){
  			if(((x-200)*(x-200)+(y-200)*(y-200))<=(r*r)){    //Shift center of circle
          
  				//Calculate unit surface normal
  				vector_n[0]= (x-200)/r;
  				vector_n[1]= (y-200)/r;
          
  				vector_n[2]= sqrt(1-((double)((x-200)*(x-200)+(y-200)*(y-200)))/(r*r));
  				
  				//Calculate angle between source S and unit surface normal N
  				theta = acos(vector_s[0]*vector_n[0]+vector_s[1]*vector_n[1]+vector_s[2]*vector_n[2]);
  				
  				//for Lambertian reflectance Ll=cos(theta), cos(theta)>=0
  				Ll = cos(theta);
  				if(Ll >= 0) {
  					//Calculate angle between surface normals N and H
  					alpha = acos(vector_h[0]*vector_n[0]+vector_h[1]*vector_n[1]+vector_h[2]*vector_n[2]);
  					//Calculate specular reflectance Ls = exp(-(alpha/m)^2)
  					Ls = exp(-1*((alpha/m)*(alpha/m)));
  					
  					//Calculate scene radiance L
  					L = a*Ll+(1-a)*Ls;
  					image_temp[y][x] = L;
  					if(max < L)
  						max = L;
                    
  				}
  			}
  		}
  	}
    /* Normalize the image array */
  	for(y=0;y<ROWS;y++){
  		for(x=0;x<COLS;x++){
  			image[y][x] = image_temp[y][x]/max*255;
  		}
  	}
    
    /* Write output to a new image*/
  	if (!( fp = fopen( filename, "wb" ) ))
  	{
  	fprintf( stderr, "error: could not open %s\n", "image" );
  	exit( 1 );
  	}
  	fwrite( head, 4, 8, fp );
  	for ( i = 0 ; i < ROWS ; i++ ) fwrite( image[i], 1, COLS, fp );
  	fclose( fp );
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}

