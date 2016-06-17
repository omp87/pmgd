#include <string.h>
#include "jarvis.h"
#include "Transaction.h"
#include "common.h"

using namespace Jarvis;

void Java_jarvis_Transaction_startTransactionNative(JNIEnv *env, jobject tx,
                                             jobject graph, jint options)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    try {
        Transaction *j_tx = new Transaction(j_db, options);
        setJarvisHandle(env, tx, j_tx);
        java_transaction = env->NewGlobalRef(tx);
    }
    catch (Exception e) {
        JavaThrow(env, e);
    }
}

void Java_jarvis_Transaction_commitNative(JNIEnv *env, jobject tx)
{
    Transaction &j_tx = *(getJarvisHandle<Transaction>(env, tx));
    try {
        j_tx.commit();
        delete(&j_tx);
        setJarvisHandle(env, tx, static_cast<Transaction *>(NULL));
        env->DeleteGlobalRef(java_transaction);
        java_transaction = NULL;
    }
    catch (Exception e) {
        JavaThrow(env, e);
    }
}

void Java_jarvis_Transaction_abortNative(JNIEnv *env, jobject tx)
{
    Transaction *j_tx = getJarvisHandle<Transaction>(env, tx);
    delete j_tx;
    setJarvisHandle(env, tx, static_cast<Transaction *>(NULL));
    env->DeleteGlobalRef(java_transaction);
    java_transaction = NULL;
}
