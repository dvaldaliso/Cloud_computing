"""Inits the database"""
import boto3, sys, os
from boto3 import resource
import time

dynamodb_resource = resource('dynamodb')


def init(table_name):
    print("Creating table %s " % table_name)
    create_table(table_name)
    print(". . .")
    print("Table created")
    add_users_row(table_name)
    print("Finished")

def create_table(table_name):
    table = dynamodb_resource.create_table(
        AttributeDefinitions=[
            {
                'AttributeName': 'eventID',
                'AttributeType': 'S'
            },
            {
                'AttributeName': 'userIdentity_userName',
                'AttributeType': 'S'
            },
            {
                'AttributeName': 'eventTime',
                'AttributeType': 'S'
            }
        ],
        TableName=table_name,
        KeySchema=[

            {
                'AttributeName': 'eventID',
                'KeyType': 'HASH'
            },
            {
                'AttributeName': 'userIdentity_userName',
                'KeyType': 'RANGE'
            }
        ],
        GlobalSecondaryIndexes=[
            {
                'IndexName': 'userIdentity_userName-eventTime-index',
                'KeySchema': [
                    {
                        'AttributeName': 'userIdentity_userName',
                        'KeyType': 'HASH'
                    },
                    {
                        'AttributeName': 'eventTime',
                        'KeyType': 'RANGE'
                    }
                ],
                'Projection': {
                    'ProjectionType': 'ALL'

                },
                'ProvisionedThroughput': {
                    'ReadCapacityUnits': 2,
                    'WriteCapacityUnits': 4
                }
            },
        ],
        ProvisionedThroughput={
            'ReadCapacityUnits': 2,
            'WriteCapacityUnits': 4
        }
    )


def add_users_row(name_table):
    print("Creating row for users, services and parameters list...")
    dynamodb_client = boto3.client('dynamodb')
    res = None
    while res is None:
        intent = 0
        try:
            table = dynamodb_resource.Table(name_table)
            datos = {
                "eventID": "1",
                "eventTime": "1",
                "userIdentity_userName": "all",
                "listUsers": {},
                "services": {},
                "cols": {}

            }
            res = table.put_item(
                Item=datos
            )
        except dynamodb_client.exceptions.ResourceNotFoundException:
            if intent > 50:
                print("Error adding users row")
                return
            intent += 1
            res = None
            print(". . .")
            time.sleep(50)



    print("Succeeded")
    return

if (__name__ == '__main__'):
    init(table_name='alucloud' + os.environ['ID'] + '-ctt')
