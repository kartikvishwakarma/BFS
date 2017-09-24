# BFS


										(PARENT NODE)

				|----------------------------|-------------|-----------|-----------|
				|(X_MIN, Y_MIN, X_MAX, Y_MAX)| SPLIT-POINT | DIRECTION | POINTER   |
				|  							 |			   |		   |		   |
				|----------------------------|-------------|-----------|-----------|
																		/\
																	   /  \	
																	  /    \
																	 /		\
																	/		 \
					(LEFT CHILD NODE)							   /		  \
																   			   \
|----------------------------|-------------|-----------|-----------|            \                                 
|(X_MIN, Y_MIN, X_MAX, Y_MAX)| SPLIT-POINT | DIRECTION | POINTER   |             \  
|  							 |			   |		   |		   |			  \
|----------------------------|-------------|-----------|-----------|			   \
																					\
																					 \
																				 	  \
																				 	   \
																				 	    \
																				 	     \(RIGHT CHILD NODE)

																	|----------------------------|-------------|-----------|-----------|
																	|(X_MIN, Y_MIN, X_MAX, Y_MAX)| SPLIT-POINT | DIRECTION | POINTER   |
																	|  							 |			   |		   |		   |
																	|----------------------------|-------------|-----------|-----------|


	


*************************************
*									*
*									*
* Compile: make  or make makefile	*
* Run: ./kartik						*
*									*
************************************



STRUCTURE:
	Spatial point representation implemented using concept similar to K-D tree.
	X-point and Y-point choosen alternaive(not always).

	Bucket of grid store at leaf node of K-D tree, each internal node entries
	 updated according to points being inserted.

	(x_min, y_min, x_max, y_max) dimention of grid and split point 
