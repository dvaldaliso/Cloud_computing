/**
modificar el ejemplo para que no se incluya esta distinción, convirtiendo
en la fase de "map" todas las palabras a minúsculas y además que separe las palabras teniendo en
cuenta diversos separadores. 
Además, sería deseable que la salida únicamente incluyera las palabras
más repetidas, por ejemplo sólo las que se repitan cinco o más veces
 */
package org.apache.hadoop.examples;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class WordCount {

  public static class TokenizerMapper 
       extends Mapper<Object, Text, Text, IntWritable>{
    
    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();
    /*
    Recibe líneas de texto y las divide en sus correspondientes palabras 
    (usando StringTokenizer). Para cada palabra, emite el par (palabra,1) indicando que dicha
    palabra ha sido encontrada con frecuencia 1. Las variables word y one están definidas 
    como atributos de la clase.
    */  
    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString(), " \"\t\n\r\f,.:;?![]()'");
      while (itr.hasMoreTokens()) {
        String token = itr.nextToken(); // get next token
        if(!token.startsWith("*")){//chequea que no comience con *
          word.set(token.toLowerCase());
          context.write(word, one);
        }
        
      }
    }
  }
  
  public static class IntSumReducer 
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();
    /*
    Recibe un conjunto de pares (palabra, frecuencia) agrupado por palabra (que es la clave),
    o lo que es lo mismo, una palabra y una lista de valores de frecuencia para dicha palabra.
    Se suma la lista de frecuencias para dicha palabra y se emite el par (palabra, frecuencia)
    como resultado de la operación.
    */    
    public void reduce(Text key, Iterable<IntWritable> values, 
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }
      result.set(sum);
      if (sum>4) {
        context.write(key, result);
      }
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length < 2) {
      System.err.println("Usage: wordcount <in> [<in>...] <out>");
      System.exit(2);
    }
    Job job = Job.getInstance(conf, "word count");
    job.setJarByClass(WordCount.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    for (int i = 0; i < otherArgs.length - 1; ++i) {
      FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
    }
    FileOutputFormat.setOutputPath(job,
      new Path(otherArgs[otherArgs.length - 1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
