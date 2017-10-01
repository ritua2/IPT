//#include "StringUtility.h"
#include "Support_function.h"
#include "MPI_functions.h"
#include "Cuda_functions.h"
#include "OpenMP_function.h"
//using namespace rose;
using namespace std;
using namespace SageBuilder;
using namespace SageInterface;

string Array;
char var[25]= "rose_";
std::string name;
int which_language =0;
int numTimes = 0;
Rose_STL_Container<SgName> completeList;

int main( int argc, char* argv[]){
        char* part = (char*)malloc(256);
        if(SgProject :: get_verbose() > 0){
                printf("//In prePostTraversal.C : main() \n");
        }
        SgProject* project = frontend(argc,argv);
        ROSE_ASSERT (project != NULL);
        SgGlobal* globalscope = getFirstGlobalScope(project);
        SgFunctionDeclaration* mainFunc = findMain(project);
        SgBasicBlock* body= mainFunc->get_definition()->get_body();
        pushScopeStack(globalscope);
        printf("\nNOTE: We currently support only C and C++ programs.\n\n");
        printf("Please select a parallel programming model from the following available options:  \n1. MPI\n2. OpenMP\n3. CUDA \n");
        cin>>which_language;
        SgBasicBlock *func_body= body;
        SgType* return_type = buildIntType();
        if (func_body != NULL)
        {
                if (func_body->get_statements().size() > 1)
                {
                        SgForStatement* loop;
                        SgNode* isloop;
                        if(which_language==1)
                        {
            	            printf("\nPlease note that by default, the MPI Enviroment Initialization functions will be set in the main function.\n\n");
            	            SgFile * cur_file = project->get_fileList()[0];
                            string orig_name = cur_file->get_file_info()->get_filenameString();
                            string file_suffix = StringUtility::fileNameSuffix(orig_name);
                            orig_name = StringUtility::stripPathFromFileName(orig_name);
                            string naked_name = StringUtility::stripFileSuffixFromFileName(orig_name);
                            cur_file->set_unparse_output_filename("rose_"+naked_name+"_MPI."+file_suffix);

                            int iteration_count = 0;
                            generalMPI(func_body);//build the MPI init size rank and finalize calls
                            buildMPIFunctions(func_body,loop);
                            int  main_pattern = 0;
                            char both;
                            bool mult = true;
                            int count=0;
			    while(mult){
                                if (main_pattern == 0) {
                                    func_body = getFunction();
                                    printf("\nPlease select a pattern from the following list that best characterizes your parallelization needs: \n(Please refer to the user-guide for the explanation on each of the patterns, and note that not all the listed patterns may be relevant for your application type.) \n1. For-Loop Parallelization \n2. Stencil \n3. Pipeline \n4. Data Distribution and Data Collection \n5. Data Distribution \n6. Data Collection \n");
                                    cin>>main_pattern;
                                }else {
                                    cout << "Do you wish to continue parallelizing the current function ?(Y/N)\n";
                                    char input2;
                                    cin >> input2;
                                    if (input2 == 'N' || input2 == 'n') {
                                        main_pattern =0;
                                        //if no further parallelization of a selected function is needed, then should one not exit from the loop?
                                        continue;
                                        //adding the break statement below temporarily - if this is not provided and contibue is there, then the question in line # 62 is repeated.
                                        //break;
                                    }
                                }
                                //the line below kept resetting the count to 0 and hence commenting it out and declaring the variable count at line # 56 
                                //int count =0;

                                if(main_pattern ==5) {
                                        SgStatement* start;
                                        SgStatement* end;
                                        printf("\nPlease enter the number of arrays that you want to perform distribution on.\n");
                                        //pushScopeStack(body);
                                        string line;
                                        printf("\nPlease enter the arrays to be split (format[1,2,3,4 etc.]). Possible arrays are :\n");
                                        getPointers(body);
                                        cin >> line;
                                        vector<string> arrays = String_spliting(line);
                                        int number_of_dis_array = arrays.size();
                                        printf("\nNOTE: For an array to be distributed properly, it must be distributed evenly across all processes. As such, please make sure that your number of array entries is divisible by the number of processes you are using.\n");
                                        for (int i1 = 0; i1 < number_of_dis_array; i1++) {
                                            int num = stoi(arrays.at(i1));
                                            SgInitializedName* name_to_replace = NULL;
                                            string array1 = getPointerVal(body,num);
                                            SgVariableSymbol*  varsym = lookupVariableSymbolInParentScopes(array1,body);
                                            name_to_replace = (varsym != NULL ) ? varsym->get_declaration() : NULL;
                                            cout << "\nArray #" << (i1+1) << " [" << name_to_replace->unparseToString() << "]" << endl; 
                                            start = insertSplitOp(name_to_replace,func_body,i1);
                                        }   
                                }else if(main_pattern ==6){
                                        int collection_type;
                                        printf("\nWould you like to collect data from: \n1. Variable  \n2. Array \n3. Both Variables and Arrays \n");
                                        cin >> collection_type;
                                        if(collection_type ==1 || collection_type==3){
                                            variableDeclarations_stencil(func_body,loop,count);
                                        }
                                        if(collection_type ==2 || collection_type==3){
                                            printf("Would you like to collect data from: \n1. Distributed stencil array \n2. Partially calculated value of an array in a for loop\n");
                                            int varCollection;
                                            cin >> varCollection;
                                            if(varCollection ==2){
                                                
                                                int val;
                                                isloop = getForStatement(func_body,project, which_language);
                                                loop = isSgForStatement(isloop);
                                                printf("Please select the array from which you want to collect data. Possible arrays are:\n");
                                                getPointers(func_body);
                                                cin >> val;
                                                char* array_name =(char*)malloc(256);
                                                strcpy(array_name,  getPointerVal(func_body,val).c_str());

                                                arrayFor(func_body,loop,count,array_name);
                                            }else{
						variableDeclarations_stencil(func_body,loop,count);
					    }
                                       }
                                }else if(main_pattern ==4){
                                        insertSplitOp(argv,body);
                                        int collection_type;
                                        printf("\nWould you like to collect data from:  \n1. Variable  \n2. Array \n3.Both Variables and Arrays\n");
                                        cin >> collection_type;
                                        if(collection_type ==1){
                                           insertReduceOp(count);
                                        }//do reduce allreduce without for loop?
                                        else if(collection_type ==2){
                                           insertReduceOp(count);
                                        }else {
					    printf("Would you like to collect data from:  \n1. Distributed stencil array \n2. Partially calculated value of an array in a for loop\n");
                                            int varCollection;
                                            cin >> varCollection;
                                            if(varCollection ==1){
                                              insertReduceOp(count);
                                              variableDeclarations_stencil(func_body,loop,count);
    				            }else{
    	                                        int val;
            	                                isloop = getForStatement(func_body,project,which_language);
    	                                        loop = isSgForStatement(isloop);
                                                printf("Please select the array from which you want to collect data. Possible arrays are:\n");
                                                getPointers(func_body);
                                                cin >> val;
                                                char* array_name =(char*)malloc(256);
                                                strcpy(array_name,  getPointerVal(func_body,val).c_str());

                                                arrayFor(func_body,loop,count,array_name);
                                           }
                                        }
                                
				}else if(main_pattern ==2 && iteration_count ==0){
                                        printf("\nNOTE: Currently supported stencil operations are with 2 dimensional matrices.\n");
                                        printf("\nThe stencil pattern can be used with or without for loop parallelization.  Would you like to parallelize the for-loop as well?(Y/N) ");
                                        cin >> both;
                                        insertHeaders_stencil(globalscope);
                                        //stencil_generalOps(func_body);
                                        iteration_count ++;
                                }
                                //iteration_count ++;
                                if(main_pattern==1 || both =='y' || both =='Y'){
                                        //for loop parallelization code
                                        generalMPI(func_body);
                                        isloop = getForStatement(func_body,project, which_language);
                                        if (isloop!=NULL){
                                           loop = isSgForStatement(isloop);
                                           //buggy
                                            SgStatement* upper = getLoopCondition(loop);
                                            SgStatementPtrList& initStatements = loop->get_init_stmt();
                                            SgStatement* lower = (!initStatements.empty()) ? initStatements.front() : NULL;
                                            #ifdef DEBUG
                                              cout << "lower is " << lower->unparseToString() << "\n";
                                            #endif 
                                            SgExpression* upperbound =isSgBinaryOp(isSgExprStatement(upper)->get_expression())->get_rhs_operand_i();
                                            SgExpression* lowerbound ;
                                            if (isSgExprStatement(lower)) {
                                               SgAssignOp* assign;
                                               SgExpression* exp =isSgExprStatement(lower)->get_expression();
                                               assign = isSgAssignOp(exp);
                                               if (assign) {
                                                  lowerbound = assign->get_rhs_operand();
                                               }                                       
                                               else {
                                                  // handling case when the init statement contains only the variable
                                                  lowerbound = exp;
                                               }
                                           }
                                           // handling the case when the init statement of the for loop is a declaration
                                           else if (isSgVariableDeclaration(lower)){
                                                SgInitializedNamePtrList& list_of_variables  = isSgVariableDeclaration(lower)->get_variables();
                                                if (list_of_variables.empty() || list_of_variables.size() > 1) {
                                                   cout << "We are not handling this case yet !!\n";
                                                }else {
                                                   SgInitializer* initializer = (list_of_variables.front())->get_initptr() ; 
                                                   if (isSgAssignInitializer(initializer)) {
                                                      lowerbound = isSgAssignInitializer(initializer)->get_operand();
                                                       #ifdef DEBUG
                                                         cout << "lowerbound is " << lowerbound->unparseToString() << endl;
                                                       #endif 
                                                   }else {
                                                       cout << "we are not handling this case yet !!\n";
                                                   }
                                                }
                                           }
                                           // handling other cases 
                                           else  {
                                                if (lower == NULL) {
                                                   SgExpression* increment = loop->get_increment();
                                                   if (isSgAssignOp(increment)) {
                                                      lowerbound = isSgAssignOp(increment)->get_lhs_operand();
                                                   }
                                                   // TODO: there are more cases to handle
                                                   else {
                                                      cout << "we are not handling this case yet !!\n";
                                                   }
                                                }
                                                // TODO: there maybe more cases to handle
                                                else {
                                                   cout << "we are not handling this case yet !!\n";
                                                }
                                        
                                           }
                                           upperAndLowerLimitValues_MPI(upperbound,lowerbound,loop,count,func_body);
                                    
                                           //data collection code
                                           int collection_type;
                                           printf("\nPlease specify the type of the data storage (variable, array, structure) from which the data collection should be done at the hotspot for parallelization:\n1. Variable/s\n2. Array/s\n3. Structure/s (not supported currently)\n4. Both Variable/s and Array/s\n5. Both Variable/s and Structure/s (not supported currently)\n6. Variable/s, Array/s, Structure/s (not supported currently)\n7. None of the above\n");
                                           cin >> collection_type;

                                           if((collection_type ==2 || collection_type ==4)){
                                               SgBasicBlock* function = func_body;
                                               printf("Please select the arrays from which you want to collect data (format 1,2,3,4 etc. with 1 is the first array in the list, 2 is the second array in the list etc.)\nPossible arrays are:\n");
                                               getPointers(function);  
                                               string str;
                                               cin >> str;
                                               vector<string> list_of_arrays = String_spliting(str);
                                        
                                               for (int i = 0; i < list_of_arrays.size();i++) {
                                                   printf ("For array #%d\n",i);
                                                   printf("Would you like to collect data from:  \n1. Distributed stencil array \n2. Partially calculated value of an array in a for loop\n");
       	        	                           int varCollection;
        	                                   cin >> varCollection;
      				                   if(varCollection ==2){
                                                      char* array_name =(char*)malloc(256);
                                                      int value = stoi(list_of_arrays.at(i));
                                                      strcpy(array_name,  getPointerVal(function,value).c_str());
                                                      #ifdef DEBUG
                                                         cout << "array name is " << array_name << endl;
                                                      #endif
                                                      arrayFor(function,loop,count,array_name);	
                                                   }  
                                                   else{
                                                      insertCollectOp(argv,func_body);
                                                   }
                                               }
                                           }
                                           // if ((collection_type==4 ||collection_type ==1) && main_pattern ==2)
                                           // {
                                           //         variableDeclarations_stencil(func_body,loop,count);
                                           //         insertExchange(argv,func_body);
                                           // }
                                           if(collection_type ==1 ||collection_type==4){
                                               variableDeclarations_MPI(func_body,loop,count);
                                           }
                                           if (collection_type == 3 || collection_type == 5 || collection_type == 6) {
                                               cout  << "Sorry! We are currently not supporting this option yet\n";
                                           }
                                           #ifdef DEBUG
                                               cout << "here\n" << endl;
                                           #endif
                                           setForLoopBounds_MPI(loop,count);
                                        }//closing the check for isloop
                                }
                                if(main_pattern ==2 && (both =='N' || both =='n'))
                                {
                                        int pattern,collection_type,line;
                                        
                                        printf("\nNOTE: Data Distributions is categorized as breaking a matrix into smaller matrices.  As such we will prompt for exchange operation regardless of chosen prompt.\n");
                                        printf("\nWould you like to do this pattern with \n1. Data collection  \n2. Data Distribution. \n3. Data collection and distribution  \n4. Neither data collection or distribution \n");
                                        cin >> pattern;
                                        if(pattern==2||pattern ==3)
                                        {  
                                            SgStatement* start;
                                            SgStatement* end;
                                           
                                            printf("\nPlease enter the number of arrays that you want to perform distribution on.\n");
                                             
                                            //pushScopeStack(body);
                                            string line;
                                            printf("\nPlease enter the arrays to be split (format[1,2,3,4 etc.]). Possible arrays are :\n");
                                            getPointers(body);
                                            cin >> line;
                                            vector<string> arrays = String_spliting(line);
                                            int number_of_dis_array = arrays.size();
                                            printf("\nNOTE: For an array to be distributed properly, it must distributed evenly across all processes.  As such, please make sure that your number of array entries is divisible by the number of processes you are using.\n");
                                            for (int i1 = 0; i1 < number_of_dis_array; i1++) {
                                                int num = stoi(arrays.at(i1));
                                                SgInitializedName* name_to_replace = NULL;
                                                string array1 = getPointerVal(body,num);
                                                SgVariableSymbol*  varsym = lookupVariableSymbolInParentScopes(array1,body);
                                                name_to_replace = (varsym != NULL ) ? varsym->get_declaration() : NULL;
                                                cout << "\nArray #" << (i1+1) << " [" << name_to_replace->unparseToString() << "]" << endl; 
                                                start = insertSplitOp(name_to_replace,func_body,i1);
                                            }   
                                        }

                                        if(pattern !=4 && pattern!=2)
                                        {   
                                                SgStatement* stmt;
                                                printf("\nWould you like to collect data from: \n1. Variable\n2. Array \n3. Both Varaible and Array\n");
                                                cin >> collection_type;
                                                if(collection_type ==2||collection_type==3){
                                                        stmt = insertCollectOp(argv,func_body);
                                                }
                                                if (collection_type ==1 ||collection_type==3)
                                                {
                                                        stmt = variableDeclarations_stencil(func_body,loop,count);
                                                }

                                        }
                                        insertExchange(argv,func_body);
                                }


                                cout << "Would you like to do this MPI pattern again?(Y/N) "<<endl;
                                char input;
                                cin >> input;
                                if(input == 'N'||input =='n'){
                                        mult = false;
                                }else {
                                       count++;
                                       //printf("\nCHECK COUNT CHECK: %d\n", count);
                                }

                        }
                        printf("\nAre you writing anything?(Y/N) \n");
                        char input;
                        cin >> input;
                        if(input =='y'||input =='Y')
                        {
                            printf("\nWould you like to use \n1.one process to do the writing (for printing the results to the standard output or writing to a file)\n2.multiple processes to do the writing (to a file)\n3.none of the above\n");
                            int numProcesses;
                            cin >> numProcesses;
                            if(numProcesses ==1)
                            {
                                char more = 't';
                                printf("\nNOTE: To make sure that one process is writing to the data, we will need to know at which lines you are writing data. \n");
                                do {
                                    if (more == 't') {
                                        string input;
                                        printf("\nIs the writing happening from the same function that is being parallelized? (y/n)\n");
                                        cin >> input;
                                        if (input == "n" || input == "N") {
                                            func_body = getFunction();
                                        }
                                    }
                                    vector<SgStatement*> exc_vec = get_vector_statment_numberCode(func_body);
                                    for (int i =0; i < exc_vec.size();i++) {
                                        SgStatement* exc = exc_vec.at(i);
                                        attachArbitraryText(exc,"if(rose_rank==0)",PreprocessingInfo::before);
                                    }
                                    printf("\nDo you have any other data output calls?(Y/N) \n");
                                    cin >> more;
                                } while(more == 'y' || more =='Y');
                            }
                        }
                        insertHeaders(globalscope);
                    }                                                                                                                                                                                                                                                               	
                    else if (which_language ==2)
                    {
                        //get input source file name and prepare output file name
                        SgFile * cur_file = project->get_fileList()[0];
                        string orig_name = cur_file->get_file_info()->get_filenameString();
                        string file_suffix = StringUtility::fileNameSuffix(orig_name);
                        orig_name = StringUtility::stripPathFromFileName(orig_name);
                        string naked_name = StringUtility::stripFileSuffixFromFileName(orig_name);
		
                        cur_file->set_unparse_output_filename("rose_"+naked_name+"_OpenMP."+file_suffix);

		        //bool forlooppara = false;
		        //char forlooppar;
                      
                        cout << "\n\nNOTE: As per the OpenMP standard, the parallel regions can have only one entry point and only one exit point.";
                        cout << "\nBranching out or breaking prematurely from a parallel region is not allowed.\n";
                        cout << "Please make sure that there are no return/break statements in the region selected for parallelization. However, exit/continue statements are allowed.\n\n";   
                        char parOption; 
			cout<< "\n\nA list containing the functions in the input file will be presented, and you may want to select one function at a time to parallelize it using multi-threading."<<endl;
                        func_body = getFunction();                     

                        cout<< "\n\nPlease select one of the following options (enter 1 or 2 or 3) " << endl;
                        cout<< "1. Create a parallel region (a group of threads will be created and each thread will execute a block of code redundantly but in parallel) "<<endl;
                        cout<< "2. Parallelize a for-loop (a group of threads will be created and each thread will execute a certain number of iterations of a for-loop)" <<endl;
                        cout<< "3. Create a parallel section (TBD - this mode is currently unavailable)"<<endl; 

                        cin >> parOption;

                        //if(parOption == '1'){
			//   cout << "Parallel region" << endl;
                        //} 
                        //if(parOption == '2'){
                        //   cout << "Loop" << endl;
                        //}
                        //if(parOption == '3'){
                        //   cout << "TBD - Section" << endl;
                        //}
  

                        bool again = true;
                        bool second = false;

                        while(again && (parOption == '1')){
                           if (second){
                               func_body = getFunction();
                            }
                            second = true;
                            
                            SgStatement *parstmts = getFirstStatement(func_body,project); 
                            if(parstmts!=NULL){
                                cout << "Some statement is found.\n";
                                cout << "\nblock:\n";
            			while (parstmts != getLastStatement(body)) {
                			cout << parstmts->unparseToString() << endl;
                			parstmts = getNextStatement(parstmts);
            			}
            			cout << "\n";
                            }else{
                                cout << "No region selected yet for parallelization.\n" ;
                            }
                            printf("\nWould you like to parallelize another region in the previously selected function or another one?(Y/N)\n");
                            char c;
                            cin >>c;
                            if(c == 'n' || c =='N'){
                                again = false;
                            }

                        } 

                        //if for-loop parallelization is required then iterate over all the for-loops  
                        while(again && (parOption == '2')){
                            if (second){
                               func_body = getFunction();
                            }
                            second = true;
            		    SgExpression* forTest;
                            isloop = getForStatement(func_body,project, which_language);
                            if (isloop!=NULL){
                                loop = isSgForStatement(isloop);
                                forTest = loop->get_test_expr();
                                insertPragmas(loop,func_body,part,parOption);
                            
        	                if(strstr(isSgNode(forTest)->unparseToString().c_str(),"&")!=NULL){
                                    Rose_STL_Container<SgNode*> loopExpressions = NodeQuery::querySubTree(loop->get_test_expr(),V_SgExpression);
                                    Rose_STL_Container<SgNode*> ::iterator i = loopExpressions.begin()+2;
                                    SgTreeCopy copyHelp;
                                    SgExpression* forLoopExp;
                                    forLoopExp = isSgExpression(isSgExpression(*(i-1))->copy(copyHelp));

                                    while(i!= loopExpressions.end())
                                    {
                                        if(strstr(isSgNode(*i)->unparseToString().c_str(),")"))
                                        {
                                                attachArbitraryText(loop->get_loop_body(),"if("+isSgNode(*i)->unparseToString()+")",PreprocessingInfo::before);
                                                break;
                                        }
                                        i++;
                                    }
                                    replaceExpression(isSgExpression(*(loopExpressions.begin())),forLoopExp);
                                    cout<< isSgNode(loop->get_test_expr())->unparseToString()<<endl;
                                }
                            }else{
                                cout << "No for-loop selected to parallelize yet.\n" ;
                            }
                            printf("\nWould you like to parallelize another loop in the previously selected function or another one?(Y/N)\n");
                            char c;
                            cin >>c;
                            if(c == 'n' || c =='N'){
                                again = false;
                            }
                        }
	                printf("\nAre you writing/printing anything from the parallelized region of the code?(Y/N) \n");
                        char input;
                        cin >> input;
                        if(input =='y'||input =='Y')
                        {
                            printf("\nWould you like to use \n1.one thread to do the writing (for printing the results to the standard output or writing to a file)\n2.multiple threads to do the writing (to a file)\n3.non of the above\n");
                            int numProcesses;
                            cin >> numProcesses;
                            if(numProcesses ==1)
                            {
                                char more = 't';
                                printf("\nNOTE: To make sure that one thread is writing the data, we will need to know at which lines you are writing the data in.\n");
                                do {
                                    string input;

                                    if (more == 't') {
                                        printf("\nIs the writing happening from the same function that is being parallelized? (y/n)\n");
                                        cin >> input;
                                        if (input == "n" || input == "N") {
                                            func_body = getFunction();
                                        }
                                    }
                                    vector<SgStatement*> exc_vec = get_vector_statment_numberCode(func_body);
                                    for (int i = 0; i < exc_vec.size();i++) {
                                        SgStatement* exc = exc_vec.at(i);
                                        SgPragmaDeclaration* prag = buildPragmaDeclaration("omp single");
                                        insertStatementBefore(exc,prag);
                                    }
                                    printf("\nDo you have any other data output calls?(Y/N) \n");
                                    cin >> more;
                                }while(more == 'y' || more =='Y');
                            }
                        }
                        insertHeaders(globalscope);
            }
            else if (which_language ==3)
            {
                    //insert CUDA code

				CUDATranslations();
				SgFile * cur_file = project->get_fileList()[0];
                                string orig_name = cur_file->get_file_info()->get_filenameString();
				string file_suffix = StringUtility::fileNameSuffix(orig_name);
    				orig_name = StringUtility::stripPathFromFileName(orig_name);
    				string naked_name = StringUtility::stripFileSuffixFromFileName(orig_name);
    				cur_file->set_unparse_output_filename("rose_"+naked_name+".cu");
                            //CUDATranslations();
                }
            }
        }
        printf("\nRunning Consistency Tests\n");
        #ifdef DEBUG
            SgStatement* stmt = getFirstStatement(body);
            cout << "\nblock:\n";
            while (stmt != getLastStatement(body)) {
                cout << stmt->unparseToString() << endl;
                stmt = getNextStatement(stmt);
            }
            cout << "\n";
        #endif
        AstTests :: runAllTests (project);
        project->unparse();

}





























