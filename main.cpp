#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
using namespace std;
using namespace cv;

//Funcion principal
int main(){
    //Variables
    Mat src,src_gray;
    //Arvhicos XML
    CascadeClassifier eye_cascade= ("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_eyepair_big.xml");//eyes
    CascadeClassifier nose_cascade= ("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_nose.xml");//nose
    CascadeClassifier face_cascade= ("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml");//face

    //Rectangulos con caracteristicas detectadas
    vector<Rect> eyes;
    vector<Rect> noses;
    vector<Rect> faces;

    VideoCapture captura_video(1);//SELECCIONAR EL PUERTO DE LA CAMARA
    Point centere,centern,centerf;

    while(true){
        captura_video >> src;
        //Validacionde la lectura del video
        if(src.empty()){
            cout<<"Error leyendo camara, verifique el puerto"<<endl;
            return -1;
        }
        ///Tratamiento de la imagen
        flip(src,src,1);
        resize(src,src,Size(640,480));
        cvtColor( src, src_gray, CV_BGR2GRAY );
        equalizeHist( src_gray, src_gray);
        ///Para las caras
        face_cascade.detectMultiScale( src_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        ///Validacion de las caracteristicas
        //Coordenadas de las puntos
        if(faces.size()==0)
            putText(src," There're no faces in the screen ",Point(0,40),2,0.40,Scalar(0,255,0),1,CV_AA);

        for(size_t k=0; k<faces.size(); k++){
            ///Corrdenadas del centro de la cara
            int comp_x=faces[k].x;
            int comp_y=faces[k].y;
            //Centro de cara
            centerf=Point(faces[k].x+int(faces[k].width*0.5), int(faces[k].y+faces[k].height*0.5));
            ///Recortamos la cara y la analizamos a partir de aqui
            Mat crop(src_gray, faces[k]);
            //Detectores de Escala
            nose_cascade.detectMultiScale(crop, noses, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10), Size(100,100));
            eye_cascade.detectMultiScale( crop, eyes,  1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, Size(10, 10), Size(300,300));
            //Solo detecta una cara
            if(noses.size()>0&&eyes.size()>0&&faces.size()>0){
                ellipse(src, centerf, Size(faces[k].width*0.40,faces[k].height*0.55),0,0,360,Scalar(0,0,255),2,8,0);
                //Punto centro
                circle(src,centerf,5,Scalar(0,255,255),-1);
                //Para las narices
                for(size_t j=0; j<noses.size(); j++){
                    centern=Point(noses[j].x+noses[j].width*0.5+comp_x, noses[j].y+noses[j].height*0.5+comp_y);
                    if(centern.y>centerf.y)
                        ellipse(src, centern, Size(noses[j].width*0.35,noses[j].height*0.25),0,0,360,Scalar(0,255,0),2,8,0);
                }
                //Para los ojoes
                for(size_t j=0; j<eyes.size(); j++){
                    centere=Point(eyes[j].x+eyes[j].width*0.5+comp_x, eyes[j].y+eyes[j].height*0.5+comp_y);
                    if(centere.y<centerf.y)
                        ellipse(src, centere, Size(eyes[j].width*0.5,eyes[j].height*0.5),0,0,360,Scalar(232,168,0),2,8,0);
                }
                ///Salida de datos
                putText(src," Detected Faces: "+to_string(faces.size()),Point(0,40),2,0.40,Scalar(0,255,0),1,CV_AA);
                putText(src,"P[x,y]=["+to_string(comp_x)+","+to_string(comp_y)+"]", Point(centerf.x+5,centerf.y+15), 2,0.40,Scalar(255,255,255),1,CV_AA);
                }
        }
        //Muestra de imagenes
        putText(src," FACE and Features Detector",Point(0,20),2,0.40,Scalar(0,255,0),1,CV_AA);
        putText(src,"@FerBlogs",Point(570,460),2,0.40,Scalar(0,255,255),1,CV_AA);
        namedWindow("Imagen en Vivo",CV_WINDOW_AUTOSIZE);
        imshow("Imagen en Vivo",src);
        //Setencia de Cierre
        int c=waitKey(1);
        if(c==27) break;
    }
    destroyAllWindows();
    return 0;
}
