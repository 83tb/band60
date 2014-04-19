from django.contrib.auth.models import User, Group
from quickstart.models import TimeSeries, Person
from rest_framework import serializers


class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('url', 'username', 'email', 'groups')


class GroupSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Group
        fields = ('url', 'name')



class TimeSeriesSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TimeSeries
        fields = ('id','when', 'xvalue', 'yvalue','zvalue','person','tags_string')

class PersonSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Person
        fields = ('name','age','gender')
