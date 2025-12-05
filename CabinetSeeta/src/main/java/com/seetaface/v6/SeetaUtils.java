package com.seetaface.v6;

import android.app.Application;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

import kotlin.jvm.JvmStatic;

/**
 * 工具类
 */
public final class SeetaUtils {
    private static final String TAG = "SeetaUtils";

    public static final String SEETA_FACE_MODEL_PATH = "/SeetaFace6/models/";

    public static Context mContext;

    @JvmStatic
    public static void init(Application application) {
        synchronized (TAG) {
            mContext = application.getApplicationContext();
        }
    }

    public static String getSeetaFaceModelPath(String model) {
//        String rootPath = mContext.getExternalFilesDir(null).getAbsolutePath();
//        String rootPath = mContext.getFilesDir().getAbsolutePath();
        //路径/sdcard/android/data/com.cabinet.toolsapp/files/Download
        String rootPath =  mContext.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
//        System.out.println("网络导入用户信息 模型存储路径 = " + rootPath);
        //路径/storage/emulated/0/Android/data/com.cabinet.toolsapp/files
//        String rootPath = context.getFilesDir().getAbsolutePath();
        return rootPath + SEETA_FACE_MODEL_PATH + model;
    }

    public static void copyFilesToPath(InputStream in, File destFile) {
        try {
            BufferedInputStream bis = new BufferedInputStream(in);
            FileOutputStream fos = new FileOutputStream(destFile);
            byte[] buffer = new byte[1024];
            int byteCount;
            while ((byteCount = bis.read(buffer)) != -1) {
                fos.write(buffer, 0, byteCount);
            }
            fos.flush();
            bis.close();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String[] getAssetsModels(Context context) {
        AssetManager assetManager = context.getAssets();
        String[] models = null;
        try {
            models = assetManager.list("models");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return models;
    }

    public static void copyModelsToExternalStorage(Context context) {
        // have assets models
        String[] assetsModels = getAssetsModels(context);
        if (assetsModels == null || assetsModels.length == 0) {
            return;
        }

        // create root directory
//        String modelPath = mContext.getExternalFilesDir(null).getAbsolutePath() + SEETA_FACE_MODEL_PATH;
//        String modelPath = mContext.getFilesDir().getAbsolutePath() + SEETA_FACE_MODEL_PATH;
        //路径/sdcard/android/data/com.cabinet.toolsapp/files/Download
        String modelPath = mContext.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath() + SEETA_FACE_MODEL_PATH;
        File modelPathFile = new File(modelPath);
        if (!modelPathFile.exists()) {
            modelPathFile.mkdirs();
        }

        if (assetsModels != null && assetsModels.length > 0) {
            try {
                AssetManager assetManager = context.getAssets();
                for (String model : assetsModels) {
                    File destModelFile = new File(modelPath + model);
//                    System.out.println("网络导入用户信息 初始化人脸识别模型 shouldCopyAssetsModels model:" + model + ", destModelFile:" + destModelFile);
                    if (!destModelFile.exists()) {
                        String fileName = "models/" + model;
//                        System.out.println("网络导入用户信息 初始化人脸识别模型 copyModelsToExternalStorage fileName:" + fileName);
                        copyFilesToPath(assetManager.open(fileName), destModelFile);
                    } else {
//                        System.out.println("网络导入用户信息 初始化人脸识别模型 存在");
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static boolean shouldCopyAssetsModels(Context context) {
//        String modelPath = mContext.getExternalFilesDir(null).getAbsolutePath() + SEETA_FACE_MODEL_PATH;
//        String modelPath = mContext.getFilesDir().getAbsolutePath() + SEETA_FACE_MODEL_PATH;
        String modelPath = mContext.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath() + SEETA_FACE_MODEL_PATH;
        File modelPathFile = new File(modelPath);
        if (modelPathFile.exists()) {
//            System.out.println("网络导入用户信息 FaceEngineHelper 模型目录");
            String[] assetsModels = getAssetsModels(context);
            if (assetsModels == null || assetsModels.length == 0) {
//                System.out.println("网络导入用户信息 FaceEngineHelper 模型不存在");
                // never copy models
                return false;
            }

            /*for (String assetsModel : assetsModels) {
                Log.d(TAG, "face--- shouldCopyAssetsModels assetsModel:" + assetsModel);
            }*/

            String[] models = modelPathFile.list();
            if (models == null || models.length != assetsModels.length) {
//                System.out.println("网络导入用户信息 FaceEngineHelper 模型存在");
                return true;
            }

            /*for (String model : models) {
                Log.d(TAG, "face--- shouldCopyAssetsModels model:" + model);
            }*/

            // sort and compare array
            Arrays.sort(assetsModels);
            Arrays.sort(models);
            if (Arrays.equals(assetsModels, models)) {
                return false;
            }
        }
        return true;
    }
}
