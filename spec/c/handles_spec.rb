require 'v8/init'

describe 'handles' do
  describe '#HandleScope' do
    let(:isolate) { V8::C::Isolate.New }

    it 'can allocate handle scopes' do
      V8::C::HandleScope(isolate) do
      end
    end

    it 'isn\'t the end of the world if a ruby exception is raised inside a HandleScope' do
      begin
        V8::C::HandleScope(isolate) do
          raise 'boom!'
        end
      rescue StandardError => e
        expect(e.message).to eq 'boom!'
      end
    end
  end
end
