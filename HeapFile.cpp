#include "HeapFile.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

HeapFile::HeapFile(): f(), curPage(), curPageIndex(0){

    cout<<"HeapFile constructor !" << endl;
}

HeapFile::~HeapFile(){

    cout<<"HeapFile Destructor !" << endl;
    delete f;
    delete curPage;
    delete curPageIndex;


}
int HeapFile::Create (char* fpath, fType file_type, void* startup){

    assert(heap == f_type);
    f.Open(0, fpath);
    return 1;

}
int HeapFile::Open(char* fpath){

    f.Open(1, fpath);
    MoveFirst();
    return 1;

}
int HeapFile::Close (){

    int f_size = f.Close();
    if(f_size >= 0) return 1;
    else return 0;


}

void HeapFile::Load (Schema& myschema, char* loadpath){




}

void HeapFile::MoveFirst (){

    // get the first page index
    curPageIndex = (off_t) 0;
    if( 0 != f.GetLength()) f.GetPage(&curPage,curPageIndex);
    else curPage.EmptyItOut();


}
void HeapFile::Add (Record& addme){

    Page tempPage;
    cout << "File length: " << f.GetLength() << endl;

    if(0 != f.GetLength()){

        tempPage = f.getPage(&tempPage, f.GetLength() - 2); //get the last page in the file
        if(0 == tempPage.Append(&addme)){// the last page is full, add record to a new page and insert into the end of file

            tempPage.EmptyItOut();
            tempPage.Append(&addme);
            f.AddPage(&tempPage, f.GetLength() - 1);

        }else{// the last page is not full, add record into the last page.

            f.AddPage(&tempPage, f.GetLength() - 2);

        }



    }else{ // This file is a new file with no page.

        if(1 == tempPage.Append(&addme)) f.AddPage(&tempPage, 0);
        else cout<<"A new page is full. Can't insert record!"<< endl;

    }



}
int HeapFile::GetNext (Record& fetchme){

    if( 0 == curPage.GetFirst(&fetchme)){// there is no next record for the currentpage

        curPageIndex++;
        cout <<"curPage in file: " << curPageIndex + 1 << endl;
        cout <<"last page Index in file" << f.GetLength() - 1 << endl;
        if(curPageIndex + 1 <= f.GetLength() - 1){ // there exists more page in the file

            f.GetPage(&curPage, curPageIndex);
            int result = curPage.GetFirst(&fetchme);
            assert( 1 == result);
            return 1;


        }else{// there exists no more page in the file

            cout<< "fail, no more page in the file";
            return 0;

        }

    }else{ // there exists next record, we have retrieved this record

        return 1;


    }



}
int HeapFile::GetNext (Record& fetchme, CNF& cnf, Record& literal){

    ComparisonEngine comp;
    while(1 == GetNext(fetchme)){

        if(1 == comp.Compare(&fetchme, &literal, &cnf)) return 1;
    }
    return 0;


}
