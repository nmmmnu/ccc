DBC_NAME=pgsql

#PostGreSQL native library is:
pgSQL_Lib= /usr/local/pgsql/lib/libpq.a
DBC_LIB= $(pgSQL_Lib)
DBC_INC_DIR= -I /usr/local/pgsql/include
   