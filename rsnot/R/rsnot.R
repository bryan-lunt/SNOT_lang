#library("rjson")

#' Load a SNOT object from a string.
#' 
#' 
#' @param snot_string A string containing a complete SNOT/JSON object.
#' @return An object built of basic R types reflecting the structure of the SNOT data in the string.
#' @examples
#' snot_load('{"a":1.0, "b":[2.0,3.0,{"c":3.14159}]}')

snot_load <- function(snot_string) {
	rjson::fromJSON(snot_string, simplify=FALSE)
}


#' Dump a SNOT object to a string.
#' 
#' 
#' @param snot_object A hierarchical R object built of basic R datatypes to be dumped to SNOT/JSON.
#' @return A string representation of the data provided.
#' @examples
#' snot_dump(c(1,2,3,4))

snot_dump <- function(snot_object) {
	rjson::toJSON(snot_object)
}


#' Get all the leaf values into a vector.
#' 
#' 
#' @param the_structure  A hierarchical R object built of basic R datatypes (A SNOT object).
#' @return A vector with all of the ordered values of the leaves of the_structure.

snot_traverse <- function(the_structure) {
    
    storage <- c()
    
    for( i in 1:length(the_structure) ){
        one_element = the_structure[[i]]
        if(typeof(one_element) == "double"){
            storage = append(storage, c(one_element))
        }else{
            storage = append(storage, snot_traverse(one_element))
        }
    }
    
    
    return(storage)
}

snot_populate_helper <- function(the_structure, a_vector){
    #print("IN ")
    #print(str(the_structure))
    final_structure = the_structure
    number_consumed = 0
    N = length(a_vector)
    
    for( i in 1:length(final_structure)){
        
        #print(str(final_structure[[i]]))
        if(typeof(final_structure[[i]]) == "double"){
            #print("Encountered leaf")
            number_consumed = number_consumed + 1
            final_structure[[i]] = a_vector[[number_consumed]]
        }else{
            #print("RECURSIVE CALL")
            pop_return = snot_populate_helper(final_structure[[i]], a_vector[number_consumed+1:N])
            final_structure[[i]] = pop_return[[1]]
            number_consumed = number_consumed + pop_return[[2]]
        }
    }
    
    #print("OUT ")
    #print(str(final_structure))
    return(list(final_structure, number_consumed))
}

#' Assign values to a SNOT object from values in a vector.
#' 
#' 
#' @param the_structure  A hierarchical R object built of basic R datatypes (A SNOT object).
#' @param a_vector An ordered vector with all of the values to assign to leaves in the structure.
#' @return A SNOT object matching the structure of the_structure, but with leaves having values taken from a_vector.
snot_populate <- function(the_structure, a_vector){
    N = length(a_vector)
    helper_return = tryCatch({
        snot_populate_helper(the_structure, a_vector)
        }, simpleError = function(e){
            #print(e$message)
            #return(list(e,0))
            #print(names(e))
            if(e$message == "subscript out of bounds"){
                stop("The vector did not have enough elements to populate the SNOT object.")
            }else{
                stop(e)
            }
        })
    
    if( N > helper_return[[2]]){
        stop("Not all elements in the supplied vector were used to populate leaves of the SNOT object.")
    }
    
    return(helper_return[[1]])
}