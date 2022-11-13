// Author : phoenix91
// date : 13th nov 2022

#include <bits/stdc++.h>
using namespace std ; 

const int bucket_size = 2 ; 

class bucket{

	friend class extendible_hashing ; 

	int local_depth = 1 ; 

	vector < int > arr  ; 

	bucket(){
		arr.assign( bucket_size , -1 ) ; 
	}

	int offset = 0 ; 

	public : int insert( int val ){

		if( offset >= bucket_size ){
			return 1 ; 
		}

		arr[offset] = val ; 

		offset++ ; 

		return 0 ; 

	}

	public : int del_value( int val ){

		auto it = find( arr.begin() , arr.end() , val ) ; 

		if( it == arr.end() ){
			return 1 ; 
		}

		int j = ( it - arr.begin() ) ; arr[j] = -1 ; 

		for( int i = j + 1 ; i < offset ;  i++  ) {
			
			arr[i-1] = arr[i] ; 

		}

		offset-- ; 

		return 0 ; 

	}

	public : void reset(){

		arr.assign( bucket_size , -1 ) ; 

		offset = 0 ; 

		return ; 

	}

	public : void print_bucket( ){

		for( int i = 0 ; i < bucket_size ; i ++ ){
			cout << arr[i] << ' ' ; 
		}

		cout << '\n' ; 

		return ; 

	}

}; 

class extendible_hashing {

		int global_depth = 1 ; 

		vector < bucket* > dic ; 

		public : extendible_hashing(){
			resize() ; 
			dic[0] = new bucket ; 
			dic[1] = new bucket ; 
		}

		public : void print(){

			cout << "Global depth is" << global_depth << '\n' ; 

			for( auto u : dic ){

				u->print_bucket() ;

			}
			
		}


		public : void resize( ){
			dic.resize( 1<<(global_depth)) ; 
		}

		int k_bit( int v ){

			int mod = 1 , ret = 0  ; 

			for ( int i = 0 ; i < global_depth ; i ++ ){
				ret += ( mod & v ) ; 
				mod *= 2 ; 
			}

			return ret ; 
		}

		public : int insert( int val ){

			int hash = k_bit( val ) ; 

			int does = dic[hash]->insert( val ) ; 

			if( !does ){

				return 0 ; 

			}

			if( dic[hash]->local_depth == global_depth ){

				global_depth++ ; 

				resize() ; 

				for( int j = ( 1<<(global_depth-1) ) ; j < ( 1<<(global_depth) ) ; j++ ){

					dic[j] = dic[j-(1<<(global_depth-1))] ; 

				}

			}

		    auto nw = new bucket ; 

			dic[hash]->local_depth++ ; 

			nw->local_depth = dic[hash]->local_depth ; 

			auto prev_val = dic[hash]->arr ; 

			prev_val.push_back( val ) ; 

			dic[hash]->reset() ; 

			dic[hash] = nw ; 

			for( auto y : prev_val ){

				int sd = k_bit( y ) ; 

				int did = dic[sd]->insert( y )  ; 

				if( !did ){
					continue ; 
				}

				return 1 ; 
			}

			return 0 ; 

	}

	public : int del_value(  int val ){

		int hash = k_bit( val ) ; 

		int does = dic[hash]->del_value( val ) ; 

		if( does ){

			return 1 ; 

		}

		int match_hash = ( 1<< ( global_depth) ) - 1 - hash ; 

		if( dic[hash] != dic[match_hash] && ( ((dic[hash]->offset) + 1 )* 2 < bucket_size ) && ( (dic[match_hash]->offset + 1 )* 2 < bucket_size )  ){

			if( hash > match_hash ){
				swap( hash , match_hash ) ; 
			}

			for( int i = 0 ; i < dic[match_hash]->offset ; i ++ ){
				dic[hash]->insert( dic[match_hash]->arr[i] ) ; 
			}

			dic[match_hash] = dic[hash] ; 

			dic[hash]->local_depth-- ; 

			int flag = 1 ; 

			for( int i = 0 ; i < ( 1<<(global_depth)) ; i ++ ){

				if( dic[i]->local_depth >= global_depth ){

					flag = 0 ; break ; 

				}

			} 

			if( !flag ){

				global_depth-- ; 
				
				resize() ; 

			}

		}

		return 0 ; 

	}


}; 


int main(){


	extendible_hashing f ; 

	cout << f.insert( 1 ) << '\n' ; 

	f.print() ; 

	cout << f.insert( 3 ) << '\n' ; 

	f.print() ; 

	cout << f.insert( 5 ) << '\n' ; 

	f.print() ; 

	cout << f.insert( 7 ) << '\n' ; 

	f.print() ; 

	cout << f.insert( 9 ) << '\n' ; 

	f.print() ; 

	cout << f.del_value( 9 ) << '\n' ; 

	f.print() ; 


}