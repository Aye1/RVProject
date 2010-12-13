#ifndef _Debug_H
#define _Debug_H


#define DEBUG 2

#include <string>
#include <iostream>


/**
    Classe facilitant le debug
*/
class Debug{

        static void space(int __nb) {
            for( int i = 1; i <__nb; i ++) {
                 std::cout<<" . ";
            }
        }

public :
        static void _trace(int __level, std::string __texte) {
            if (__level<DEBUG){
                space(__level);
                std::cout<<__texte<<std::endl;
            }
        }

        static void _trace(int __level, std::string __texte, void* __adresse) {
            if (__level<DEBUG){
                space(__level);
                std::cout<<__texte<<"  @"<<__adresse<<std::endl;
            }
        }


        static void _trace(int __level, char *__texte, int __nb) {
            if (__level<DEBUG){
                space(__level);
                std::cout<<__texte<<"  :"<<__nb<<std::endl;
            }
        }



};//class Debug
#endif





