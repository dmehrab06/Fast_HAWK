#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

#include "lr.h"
#include "specialfunctions.h"

#define eps 1e-30

#define PCA_COUNT 10
#define NULL_MODEL_FEATURE_COUNT 3
#define ALT_MODEL_FEATURE_COUNT 4
#define MAX_LINE_LENGTH 512
#define CLASS_NAME_LENGTH 8
#define CLASS_NAME1 "Case"
#define CLASS_NAME2 "Control"
#define CHUNK_SIZE 10
//#define DEBUG

using namespace std;

ifstream con_file;
ifstream feature_z_file;
ifstream ind_file;
ifstream total_file;

int open_file_connection();
int find_row_count();

void printmodel(vector<double>model){
    for(size_t i = 0; i<model.size(); ++i){
        cout<<model[i]<<" ";
    }
    cout<<"\n";
}

int main()
{
    if(open_file_connection()) {
        cout<<"Error in opening file"<<std::endl;
        return 0;
    }

    double learn_rate = 0.1; 
    int mx_iter = 20;
    //cin>>learn_rate>>mx_iter;

    unsigned int nrow = find_row_count();

	//Y.size() and nrow are equal
    std::vector<std::vector<double> > Z(nrow,std::vector<double>(PCA_COUNT,0));
    std::vector<double> Y(nrow);
    std::vector<unsigned long long int> totals(nrow);
    std::vector<double> counts(Y.size());
	std::vector<std::vector<unsigned long long int> > kmercounts;

	/*
	 * 51-82 does 10,17 and 22-36 of R script
	 */
    for(unsigned int l=0;l<Y.size();l++)
    {
        char buf[MAX_LINE_LENGTH];
        char class_name[CLASS_NAME_LENGTH];
        for(int l1=0;l1<PCA_COUNT;l1++)
        {
            feature_z_file>>Z[l][l1];
        }

        /*
         * because each line's 3rd string (which is last) is of importance
         * just take three string input from the line and keep the last
         */
        ind_file.getline(buf,MAX_LINE_LENGTH-1);
        stringstream sstream(buf);
        for(int l1=strlen(buf)-1;l1>=0;l1--)
        {
			// works only if class name is 3rd non alpha numeric seperated string of line\t'
            sstream>>class_name;
            sstream>>class_name;
            sstream>>class_name;
        }

        if(!strcmp(class_name,"Case")) {
            Y[l] = 1.0;
        }
        else {
            Y[l] = 0.0;
        }

        total_file>>totals[l];
    }
	//release the file connections
    feature_z_file.close();
    ind_file.close();
    total_file.close();

    #ifdef DEBUG
    cout<<"nrow : "<<nrow<<std::endl;
    cout<<"Z"<<std::endl;
    for(int l=0;l<nrow;l++)
    {
        for(int l1=0;l1<PCA_COUNT;l1++)
        {
            cout<<Z[l][l1]<<' ';
        }
        cout<<std::endl;
    }

    cout<<"Y"<<std::endl;
    for(int l=0;l<nrow;l++)
    {
        cout<<Y[l]<<' ';
    }
    cout<<std::endl;

    cout<<"totals"<<std::endl;
    for(int l=0;l<nrow;l++)
    {
        cout<<totals[l]<<' ';
    }
    cout<<std::endl;
    #endif

	/*
	 * below matrix creation is done for fitting glm using glm function
	 * 4th column of matrix will be different for each sample (as per understanding)
	 */
	int chunk_size = CHUNK_SIZE;
	
	std::vector<std::vector<double> > features_NULL(nrow,std::vector<double>(NULL_MODEL_FEATURE_COUNT+1));
	std::vector<std::vector<double> > features_ALT(nrow,std::vector<double>(ALT_MODEL_FEATURE_COUNT+1));

	for(unsigned int l=0;l<nrow;l++)
	{
		features_NULL[l][0] = 1;
		features_NULL[l][1] = Z[l][0];
		features_NULL[l][2] = Z[l][1];
		features_NULL[l][3] = totals[l];
	}

	for(unsigned int l=0;l<nrow;l++)
	{
		features_ALT[l][0] = 1;
		features_ALT[l][1] = Z[l][0];
		features_ALT[l][2] = Z[l][1];
		features_ALT[l][3] = totals[l];
	}

	std::vector<double>null_model = glm(features_NULL,Y,0.1,20);
	//printmodel(null_model);
    int chunkread = 0;
    int data = 1;
	while(true)
    {
        char buf[MAX_LINE_LENGTH];
        int read_row_count;
        kmercounts.clear();
        for(read_row_count=0;read_row_count<chunk_size;read_row_count++)
        {
			con_file>>buf>>buf>>buf>>buf;

			if(con_file.eof()) {
				break;
			}

			kmercounts.push_back(std::vector<unsigned long long int>(Y.size()));
			for(unsigned int l=0;l<Y.size();l++)
			{
				con_file>>kmercounts[read_row_count][l];
			}
		}
		//kmercounts er size protibar CHUNK_SIZE kore bartese
		//kmercounts er each row te 15 ta column

        #ifdef DEBUG
        /*
         * loop to see extraction from con_file is done correctly
         */
        cout<<"Portion of kmercounts : "<<std::endl;
        for(int l=0;l<chunk_size;l++)
        {
			for(int l1=0;l1<Y.size();l1++)
			{
				cout<<kmercounts[l][l1]<<' ';
			}
			cout<<std::endl;
		}
        #endif
        //cout<<read_row_count<<std::endl;
        /*
         * below loop is R script's %dopar% loop
         * this loop's each itration will be divided to different threads (possibly)
         * output of each iteration are arranged in a row with column count==read_row_count
         */
         //read_row_count is equal to CHUNK_SIZE
         //shudhu sesh bar ektu kom hoite pare
        for(int l=0;l<read_row_count;l++)
        {
			for(unsigned int l1=0;l1<Y.size();l1++)
			{
				counts[l1] = kmercounts[l][l1]/(double)totals[l1];
				//cout<<counts[l1]<<" ";
			}
			//cout<<"\n";
			//create the fourth column of matrix
			for(unsigned int l1=0;l1<Y.size();l1++)
			{
				features_ALT[l1][4] = counts[l1];
			}
			//glm and anova will be here (possibly)
			
            std::vector<double> alt_model = glm(features_ALT,Y,learn_rate,mx_iter);
            
            //printmodel(alt_model);
			
            double alt_likelihood = 1.0;
			for(int dat = 0; dat < features_ALT.size(); ++dat){
				std::vector<double> data(features_ALT[0].size());
				for(int j = 0; j<features_ALT[0].size(); ++j){
					data[j] = features_ALT[dat][j];
				}
				double p = predict(alt_model, data);
				//cout<<p<<"\n";
				if( Y[dat] == 1){
					alt_likelihood = alt_likelihood*p;
				}
				else{
					alt_likelihood = alt_likelihood*(1.0-p); 
				}
			}

			//calculating null likelihood
			//cout<<"null_likelihood calculation\n";
			double null_likelihood = 1.0;
			for(int dat = 0; dat < features_NULL.size(); ++dat){
				std::vector<double>data(features_NULL[0].size());
				for(int j = 0; j<features_NULL[0].size(); ++j){
					data[j] = features_NULL[dat][j];
				}
				double p = predict(null_model, data);

				if( ((int)Y[dat]) == 1){
					null_likelihood = null_likelihood*p;
				}
				else{
					null_likelihood = null_likelihood*(1.0-p); 
				}
			}

            //cout<<null_likelihood<<" "<<alt_likelihood<<"\n";


			if((null_likelihood)==0 && (alt_likelihood)==0.0) {
				null_likelihood = 0.001;
				alt_likelihood = 1.0;
			}

            double likelihood_ratio = null_likelihood/alt_likelihood;
            double log_likelihood_ratio = -2.0*(log(likelihood_ratio));

            //cout<<log_likelihood_ratio<<"\n";

            if(fabs(log_likelihood_ratio)<eps || log_likelihood_ratio<0.0){
                log_likelihood_ratio = 0.0;
            }

            cout<<alglib::chisquarecdistribution(1, log_likelihood_ratio)<<"\n";
            
            data++;
			
		}
        //cout<<read_row_count<<std::endl;
        if(read_row_count < chunk_size) {
			break;
		}
    }

    return 0;
}


int open_file_connection()
{
    con_file.open("case_out_w_bonf_top.kmerDiff");
    feature_z_file.open("pcs.evec");
    ind_file.open("gwas_eigenstratX.ind");
    total_file.open("total_kmer_counts.txt");

    if(!con_file) {
        cout<<"case_out_w_bonf_top.kmerdiff not found";
        return 1;
    }
    if(!feature_z_file) {
        cout<<"pcs.evec not found";
        return 1;
    }
    if(!ind_file) {
        cout<<"gwas_eigenstratX.ind not found";
        return 1;
    }
    if(!total_file) {
        cout<<"total_kmer_counts.txt not found";
        return 1;
    }
    return 0;
}

int find_row_count()
{
    int nrow = 0;
    char buf[MAX_LINE_LENGTH];

    total_file.getline(buf,MAX_LINE_LENGTH-1);
    while(!total_file.eof())
    {
        nrow++;
        total_file.getline(buf,MAX_LINE_LENGTH-1);
    }
    total_file.clear(); // needed before seekg if not c++11
    total_file.seekg(0,ios::beg);

	return nrow;
}
