https://artifacthub.io/

minikube start
alias "kubectl=minikube kubectl -- "

Deployment manage replicaset
Replicaset manage all replica of pod
Pod is an abstraction of a container 
Container 

- kubectl create deployment nginx-depl --image=nginx
- kubectl create deployment mongo-depl --image=mongo
- kubectl get replicaset

## edit deployment si lo edito me crea una replica 
- kubectl edit deployment nginx-depl 
- kubectl get nodes|pod|service|replicaset|deployment

-- DEBUG pods
-- este falla mientras se esta creando
- kubectl logs name-pod
-- usamos el sigueinte para ver que esta pasando 
- kubectl describe pod mongo-depl-67d7db846c-4f8ks
--interactuar con un pod
- kubectl exec -it  mongo-depl-67d7db846c-4f8ks -- bin/bash

- kubectl delete deployment mongo-depl

## Apply configuration file
- kubectl apply -f deployment/nginx-deployment.yaml
-- Delete configuration file
- kubectl delete -f deployment/nginx-deployment.yaml

- kubectl apply -f service/nginx-serv.yaml

## More info about pod
-- El siguiente comando nos muestra los detalles del servicio como ip
- kubectl describe service nginx-service
-- El siguiente comando nos muestra todos los pods con su ip
- kubectl get pod -o wide

## para mas detalles de un deployment creado
- kubectl get deployment nginx-deployment -o yaml > deployment/nginx-deployment-result.yaml

## Ejercicio general
Creamos un secret para las contrsenas, las contrasenasl las creamos en base 64
- echo -n "username" | base64
- echo -n "password" | base64

-- Creamos el secret
- kubectl apply -f secret/mongodb.yaml
- kubectl apply -f deployment/mongodb-deployment.yaml
-- El servicio lo adicionamos dentro de deployment/mongodb-deployment.yaml y lo volvimos a aplicar
-- Chequeamos con el describe para ver que las ip coinciden en la conexion

-- Listar todo que tenga que ver con mongo
- kubectl get all | grep mongodb

-- Creamos un config map para ser varaible a donde se va a conectar
- kubectl apply -f configMap/mongo-configmap.yaml

-- Creamos el deployment de mongo express
- kubectl apply -f deployment/mongo-express-deployment.yaml

-- Creamos el servicio de mongo express dentro del deployment y volvemos a ejecutarlo,
-- para que el servicio sea externo a diferencia de los demas se le type "LoadBalancer"
- kubectl apply -f deployment/mongo-express-deployment.yaml

--Luego listamos los servicios y vemos como el servico externo no le da una ip con el siguiente comando se lo da
- minikube service nombre-service
- minikube service mongo-express-service


-- NAMESPACES
- kubectl get namespace
- kubectl create namespace dev
- kubectl create namespace prod
- kubectl get namespace
- kubectl delete namespace dev


-- INGRESS
-- Para activarlo en minikube
minikube addons enable ingress

kubectl get ingress -n name-ingress --watch

-- https://www.youtube.com/watch?v=X48VuDVv0do minuto 2:17:41