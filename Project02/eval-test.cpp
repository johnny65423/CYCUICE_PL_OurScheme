  Token * Evalexp( Token * temp ) {
    if ( Isatomtype( temp->type ) && temp->type != SYMBOL )
      return temp ;
    else if ( temp->type == SYMBOL ) {
      if ( Findsymbol( temp->str ) )
        return Symbols( temp ) ;
      else if ( Isinternalfunc( temp->str ) )
        return Interfunc( temp ) ;
      else 
        throw UnboundError( temp->str ) ;
      
    } // else if
    else { // what is being evaluated is >>>(...)<<< ; we call it the main S-expression below

      if ( !Islist( temp ) ) // (...) is not a (pure) list
        throw NonListError() ;  // (...)要pretty print
      else if ( Isatomtype( temp->left->type ) && temp->left->type != SYMBOL )
          throw NonFuncError( temp->left->str ) ;
      else if ( temp->left->type == SYMBOL ) {
        string sym = temp->left->str ;
        // check whether SYM is the name of a function (i.e., check whether 「SYM has a
        //                                binding, and that binding is an internal function」)
        
        if ( !Findsymbol( sym ) )
          UnboundError( sym ) ;
        else if ( !Isinternalfunc( sym ) )
          throw NonFuncError( sym ) ;
        else { // SYM is the name of a known function

          // if ( the current level is not the top level, and SYM is 'clean-environment' or 
          // 'define' or　'exit' ) {
          //   ERROR (level of CLEAN-ENVIRONMENT) / ERROR (level of DEFINE) / ERROR (level of EXIT)
          if ( 1 == 0 ) ;
          else if ( sym == "define" || sym == "set!" || sym == "let" || 
                    sym == "cond" || sym == "lambda" ) { 
                    // SYM is 'define' or 'set!' or 'let' or 'cond' or 'lambda'

            check the format of this expression // 注意：此時尚未check num-of-arg
            // (define symbol    // 注意：只能宣告或設定 非primitive的symbol (這是final decision!)
            //         S-expression
            // )
            // (define ( one-or-more-symbols )
            //           one-or-more-S-expressions
            // )
            // (set! symbol
            //       S-expression
            // )
            // (lambda (zero-or-more-symbols)
            //           one-or-more-S-expressions
            // )
            // (let (zero-or-more-PAIRs)
            //        one-or-more-S-expressions
            // )
            // (cond one-or-more-AT-LEAST-DOUBLETONs
            // )
            // where PAIR df= ( symbol S-expression )
            //        AT-LEAST-DOUBLETON df= a list of two or more S-expressions
            /*
            if ( 1 == 0 ) { // format error (包括attempting to redefine system primitive)
              ERROR (COND format) : <the main S-exp> 
              or
              ERROR (DEFINE format) : <the main S-exp> // 有可能是因為redefining primitive之故
              or
              ERROR (SET! format) : <the main S-exp>    // 有可能是因為redefining primitive之故
              or
              ERROR (LET format) : <the main S-exp>     // 有可能是因為redefining primitive之故
              or
              ERROR (LAMBDA format) : <the main S-exp>  // 有可能是因為redefining primitive之故
            } // if
            */
            

            return Evalexp( temp ) ;
          } // else if
          else if ( sym == "if" || sym == "and" || sym == "or" ) { // SYM is 'if' or 'and' or 'or'
            /*
            check whether the number of arguments is correct

            if number of arguments is NOT correct
              ERROR (incorrect number of arguments) : if
            */
            return Evalexp( temp ) ;
          } // else if
          else {// SYM is a known function name 'abc', which is neither 
                // 'define' nor 'let' nor 'cond' nor 'lambda'
            /*
            check whether the number of arguments is correct

            if number of arguments is NOT correct
              ERROR (incorrect number of arguments) : abc
             */           
            return Evalexp( temp ) ;          
          } //  else
        } // else ( SYM is the name of a known function )

        
      else {// the first argument of ( ... ) is ( 。。。 ), i.e., it is ( ( 。。。 ) ...... )

        Token checktemp = Evalexp( temp->left ) ;

        // if any error occurs during the evaluation of ( 。。。 ), we just output an
        // an appropriate error message, and we will not proceed any further
        if ( 1 == 1 ) {

          // check whether the evaluated result (of ( 。。。 )) is an internal function
          if ( Isinternalfunc( checktemp->str ) ) {
          // the evaluated result (of ( 。。。 )) is an internal function )
            /*
            check whether the number of arguments is correct

            if num-of-arguments is NOT correct
              ERROR (incorrect number of arguments) : name-of-the-function
              or
              ERROR (incorrect number of arguments) : lambda expression 
              // in the case of nameless functions

            */
            Evalexp( temp->left ) ;
          } // if
          else // the evaluated result (of ( 。。。 )) is not an internal function
            throw NonFuncError( checktemp->left->str ) ; //  ☆ is the evaluated result
        } // if
      } // else end of 「else the first argument of ( ... ) is ( 。。。 )」
        
      /*  
      eval the second argument S2 of (the main S-expression) ( ... )

      if the type of the evaluated result is not correct 
        ERROR (xxx with incorrect argument type) : the-evaluated-result
        // xxx must be the name of some primitive function!

      if no error
        eval the third argument S3 of (the main S-expression) ( ... )

      if the type of the evaluated result is not correct 
        ERROR (xxx with incorrect argument type) : the-evaluated-result

      ...

      if ( no error ) {

        apply the binding of the first argument (an internal function) to S2-eval-result, 
        S3-eval-result, ... 

        if ( no error ) {
          if there is an evaluated result to be returned
            return the evaluated result
          else
            ERROR (no return result) : name-of-this-function
            or
            ERROR (no return result) : lambda expression // if there is such a case ...
        } // if
      } // if
      */
    } // else what is being evaluated is (...) ; we call it the main S-expression
  } // Evalexp()