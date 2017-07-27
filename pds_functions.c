#include "pds.h"
#include <stdio.h>
#include <stdlib.h>
#include "contact.h"

struct PdsInfo pdsInfo;
FILE *repo_fptr;

int pds_store( struct Contact *c1 ){

	struct Contact c2 = *c1;
	
	int status = pds_search_by_key(c2.contact_id,&c2);
	
	if(repo_fptr && (status == PDS_REC_NOT_FOUND)){
		fwrite(c1,sizeof(struct Contact),1,repo_fptr);

		return PDS_SUCCESS;
	}
	else {
		return PDS_DUPLICATE_RECORD;
	}

	return PDS_STORE_ERROR;
}

int pds_search_by_key( int key, struct Contact *c ){


    fseek(repo_fptr, 0L, SEEK_SET);
    if(pdsInfo.repo_status == 1){
    	while(fread(c, sizeof(struct Contact), 1, repo_fptr)){
	    	if(c->contact_id == key){
	    		//printf("Contact_id= %d\n", c->contact_id);
	    		return PDS_SUCCESS;
	    	}
	    }
	    return PDS_REC_NOT_FOUND;
    }
	return PDS_REPO_NOTOPEN;
}


int pds_open(char *repo_name){

	repo_fptr = fopen(repo_name,"rb+");
	if(repo_fptr){
		pdsInfo.repo_status = 1;
		return PDS_SUCCESS;
	}
		
	else{
		pdsInfo.repo_status = 0;
		return PDS_OPEN_ERROR;
	}
		
}

int pds_close(){
	fclose(repo_fptr);

	pdsInfo.repo_status = 0;

	return PDS_REPO_NOTOPEN;
}

int pds_create(char *repo_name) {
	repo_fptr = fopen(repo_name, "wb" );
	if(repo_fptr)
		return PDS_SUCCESS;
	else	
		return PDS_CREATE_ERROR;
}