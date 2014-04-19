from django.shortcuts import render

# Create your views here.
from django.contrib.auth.models import User, Group
from rest_framework import viewsets
from quickstart.models import Person, TimeSeries
from quickstart.serializers import UserSerializer, GroupSerializer, TimeSeriesSerializer, PersonSerializer


class UserViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows users to be viewed or edited.
    """
    queryset = User.objects.all()
    serializer_class = UserSerializer


class GroupViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Group.objects.all()
    serializer_class = GroupSerializer


class PersonViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Person.objects.all()
    serializer_class = PersonSerializer

class TimeSeriesViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows TimeSeries to be viewed or edited.
    """
    queryset = TimeSeries.objects.all()
    serializer_class = TimeSeriesSerializer





from rest_pandas import PandasView
from .models import TimeSeries

class TimeSeriesView(PandasView):
    model = TimeSeries
    # fields = ('when', 'xvalue', 'yvalue','zvalue')
    # In response to get(), the underlying Django REST Framework ListAPIView
    # will load the default queryset (self.model.objects.all()) and then pass
    # it to the following function.

    def filter_queryset(self, qs): 
        print self.request.GET


        try:
            timefrom = self.request.GET['from']
            timeto = self.request.GET['to']
            person = self.request.GET['person']
            person = Person.objects.get(id=person)


        # http://127.0.0.1:8000/quickstart/data/?from=432789342987423&to=43234432432234&person=1
            dqs = qs.filter(when__range=[timefrom, timeto]).filter(person=person)
            qs = dqs

        except:
            pass


        # At this point, you can filter queryset based on self.request or other
        # settings (useful for limiting memory usage)
        return qs

    # Then, the included PandasSerializer will serialize the queryset into a
    # simple list of dicts (using the DRF ModelSerializer).  To customize
    # which fields to include, subclass PandasSerializer and set the
    # appropriate ModelSerializer options.  Then, set the serializer_class
    # property on the view to your PandasSerializer subclass.

    # Next, the PandasSerializer will load the ModelSerializer result into a
    # DataFrame and pass it to the following function on the view.

    def transform_dataframe(self, dataframe):
        # Here you can transform the dataframe based on self.request
        # (useful for pivoting or computing statistics)
        return dataframe

    # Finally, the included Renderers will process the dataframe into one of
    # the output formats below.

