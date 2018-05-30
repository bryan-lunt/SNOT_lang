#library("rjson")

#Seems to work

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